#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"

qt_dir="${OPENLESSON_QT_DIR:-$HOME/Qt/6.12.0/macos}"
arch="${OPENLESSON_ARCH:-x86_64}"
build_dir="$PWD/outputs/macos-build-$arch"
release_dir="$PWD/outputs/macos-release-$arch"
app="$release_dir/OpenLesson.app"

"$qt_dir/bin/qt-cmake" \
    -S . -B "$build_dir" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="$arch"

cmake --build "$build_dir" --config Release --parallel

xattr -cr "$build_dir/OpenLesson.app"

cmake --install "$build_dir" --config Release --prefix "$release_dir"

if ! codesign --verify --deep --strict "$app"; then
    xattr -cr "$app"
    "$qt_dir/bin/macdeployqt" "$app" -always-overwrite
    codesign --verify --deep --strict "$app"
fi

printf '\nPackaged app: %s\n' "$app"