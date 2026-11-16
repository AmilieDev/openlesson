
#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

QT_DIR="${OPENLESSON_QT_DIR:-$HOME/Qt/6.12.0/macos}"

BUILD_DIR="$PWD/outputs/macos-build-universal"
RELEASE_DIR="$PWD/outputs/macos-release-universal"
APP="$RELEASE_DIR/OpenLesson.app"

echo "[OpenLesson] Configuring Universal 2 build..."

"$QT_DIR/bin/qt-cmake" \
    -S . \
    -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
    -DCMAKE_INSTALL_PREFIX="$RELEASE_DIR"

echo "[OpenLesson] Compiling..."

cmake --build "$BUILD_DIR" \
    --config Release \
    --parallel

echo "[OpenLesson] Installing..."

cmake --install "$BUILD_DIR" \
    --config Release

echo "[OpenLesson] Deploying Qt..."

"$QT_DIR/bin/macdeployqt" "$APP" \
    -always-overwrite

echo "[OpenLesson] Signing application..."

codesign --force --deep --sign - "$APP"

echo "[OpenLesson] Verifying architectures..."

lipo -info "$APP/Contents/MacOS/OpenLesson"

echo "[OpenLesson] Verifying signature..."

codesign --verify --deep --strict "$APP"

printf '\nPackaged app: %s\n' "$APP"

