import os
import sys

def process_file(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        modified_content = content.replace('\t', '    ')
        
        if not modified_content.endswith('\n'):
            modified_content += '\n'
        
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(modified_content)
        
        print(f"processed: {file_path}")
    
    except UnicodeDecodeError:
        print(f"passed (non text file): {file_path}")
    except Exception as e:
        print(f"processing eror: {file_path}: {e}")

def process_directory(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            process_file(file_path)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python code_analyzer.py <directory_to_check>")
        sys.exit(1)
    
    target_dir = sys.argv[1]
    if not os.path.isdir(target_dir):
        print(f"eror: {target_dir} is not directory")
        sys.exit(1)
    
    process_directory(target_dir)
    print("processing end")        