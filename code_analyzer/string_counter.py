import os
import sys

def count_in_file(file_path) -> int:
    base_name = os.path.basename(file_path)
    extension = os.path.splitext(base_name)[1].lower()

    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        return len(content.splitlines())
    except UnicodeDecodeError:
        print(f"passed (non text file): {file_path}")
    except Exception as e:
        print(f"processing error: {file_path}: {e}")
    return 0

def count_in_directory(directory) -> int:
    count = 0
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            count += count_in_file(file_path)
    return count

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python string_counter.py <directory_to_check>")
        sys.exit(1)
    
    target_dir = sys.argv[1]
    if not os.path.isdir(target_dir):
        print(f"error: {target_dir} is not a directory")
        sys.exit(1)
    
    print(f"Starting processing in: {target_dir}")
    count = count_in_directory(target_dir)
    print("Processing end. String count: ", count)