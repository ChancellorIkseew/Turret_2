from pathlib import Path
import sys


EXCLUDED_FILENAMES = {
    "glad.h",
    "glad.c",
    "khrplatform.h",
}

def count_in_file(file_path: Path) -> int:
    if file_path.name in EXCLUDED_FILENAMES:
        print(f"ignored: {file_path}")
        return 0

    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        return len(content.splitlines())
    except UnicodeDecodeError:
        print(f"passed (non text file): {file_path}")
    except Exception as e:
        print(f"processing error: {file_path}: {e}")
    return 0

def count_in_directory(directory_path: Path) -> int:
    count = 0
    for path in directory_path.rglob('*'):
        if path.is_file():
            count += count_in_file(path)
    return count

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python string_counter.py <directory_to_check>")
        sys.exit(1)
    
    target_dir = Path(sys.argv[1])
    if not target_dir.is_dir():
        print(f"error: {target_dir} is not a directory")
        sys.exit(1)
    
    print(f"Starting processing in: {target_dir}")
    count = count_in_directory(target_dir)
    print("Processing end. String count: ", count)
