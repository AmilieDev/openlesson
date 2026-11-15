import hashlib
from io import BytesIO
from pathlib import Path
import sys
from tempfile import TemporaryDirectory
from urllib.request import urlopen
from zipfile import ZipFile

VERSION = "1.0.7"
SHA256 = "2ffab6ebf58c7aefb0bcb3a1a385d207746823d6dd87d44bd666e1286939943e"
URL = (
    "https://github.com/acidanthera/OpenCorePkg/releases/download/"
    f"{VERSION}/OpenCore-{VERSION}-RELEASE.zip"
)

BOOT_FILES = (
    "BOOT/BOOTx64.efi",
    "OC/OpenCore.efi",
    "OC/Drivers/OpenRuntime.efi",
)

def prepare_efi(destination):
    destination = destination.absolute()

    if destination.exists() or destination.is_symlink():
        raise FileExistsError(f"EFI already exists at {destination}")

    destination.parent.mkdir(parents=True, exist_ok=True)

    print(f"Downloading OpenCore {VERSION} RELEASE…", flush=True)
    with urlopen(URL, timeout=60) as response:
        archive_bytes = response.read()

    print("Verifying the release checksum…", flush=True)
    if hashlib.sha256(archive_bytes).hexdigest() != SHA256:
        raise ValueError("OpenCore release checksum mismatch")

    with TemporaryDirectory(prefix=".opencore-", dir=destination.parent) as temporary:
        staged = Path(temporary) / "EFI"

        with ZipFile(BytesIO(archive_bytes)) as archive:
            for relative_path in BOOT_FILES:
                target = staged / relative_path
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(archive.read(f"X64/EFI/{relative_path}"))

        for name in ("ACPI", "Kexts"):
            (staged / "OC" / name).mkdir()

        if destination.exists() or destination.is_symlink():
            raise FileExistsError(f"EFI already exists at {destination}")

        staged.rename(destination)

    print(f"EFI ready at {destination}", flush=True)


if __name__ == "__main__":
    try:
        prepare_efi(Path(sys.argv[1]))
    except Exception as error:
        print(f"Could not prepare EFI: {error}", file=sys.stderr)
        sys.exit(1)