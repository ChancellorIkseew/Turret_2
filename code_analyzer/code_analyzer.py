from pathlib import Path
import sys

def process_file(file_path: Path):
    extension = file_path.suffix

    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        modified_content = content
        needs_rewrite = False

        if extension == '.hpp':
            pragma_line = "#pragma once\n"
            trimmed_content = content.lstrip()
            if not trimmed_content.startswith("#pragma once"):
                print(f"missing pragma once: {file_path}")
                start_index = len(content) - len(trimmed_content)
                modified_content = content[:start_index] + pragma_line + trimmed_content
                needs_rewrite = True

        if '\t' in modified_content:
            modified_content = modified_content.replace('\t', '    ')
            needs_rewrite = True
            
        if not modified_content.endswith('\n'):
            modified_content += '\n'
            needs_rewrite = True
            
        if needs_rewrite:
            with open(file_path, 'w', encoding='utf-8') as file:
                file.write(modified_content)
            print(f"processed: {file_path}")
    
    except UnicodeDecodeError:
        print(f"passed (non text file): {file_path}")
    except Exception as e:
        print(f"processing error: {file_path}: {e}")

def process_directory(directory_path: Path):
    for path in directory_path.rglob('*'):
        if path.is_file():
            process_file(path)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python code_analyzer.py <directory_to_check>")
        sys.exit(1)
    
    target_dir = Path(sys.argv[1])
    if not target_dir.is_dir():
        print(f"error: {target_dir} is not a directory")
        sys.exit(1)
    
    print(f"Starting processing in: {target_dir}")
    process_directory(target_dir)
    print("Processing end")
