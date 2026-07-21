#!/usr/bin/env python3
import os
import re
import json

def get_slug(name):
    # Remove leading number like "1. Two Sum" -> "Two Sum"
    name = re.sub(r'^\d+\.\s*', '', name)
    # Lowercase
    name = name.lower()
    # Remove non-alphanumeric chars except space/hyphen/underscore
    name = re.sub(r'[^a-z0-9\s_-]', '', name)
    # Replace spaces/hyphens with underscores
    name = re.sub(r'[\s-]+', '_', name)
    # Clean up duplicate underscores
    name = re.sub(r'_+', '_', name)
    return name.strip('_')

def compress_html(html):
    """
    Compresses HTML by shrinking whitespace outside <pre>...</pre> tags,
    making it safe for single-line JSON strings while preserving preformatted blocks.
    """
    # Split by <pre>...</pre> to protect code snippets and input/output examples
    parts = re.split(r'(<pre>.*?</pre>)', html, flags=re.DOTALL)
    compressed_parts = []
    for part in parts:
        if part.startswith('<pre>') and part.endswith('</pre>'):
            # Keep pre blocks intact
            compressed_parts.append(part)
        else:
            # Replace consecutive whitespace/newlines with a single space
            part_compressed = re.sub(r'\s+', ' ', part)
            compressed_parts.append(part_compressed)
    return "".join(compressed_parts).strip()

def decompress_html(compressed_html):
    """
    Returns the compressed HTML as is.
    This serves as the extractor/decompressor counterpart.
    """
    return compressed_html

def main():
    html_path = 'out.html'
    json_dir = 'problems'
    output_html_dir = os.path.join(json_dir, 'descriptions_html')
    os.makedirs(output_html_dir, exist_ok=True)

    if not os.path.exists(html_path):
        print(f"Error: {html_path} not found.")
        return

    print("Scanning JSON files in directory:", json_dir)
    json_files = []
    for root, _, files in os.walk(json_dir):
        # Skip descriptions_html folder itself
        if 'descriptions_html' in root:
            continue
        for file in files:
            if file.endswith('.json'):
                json_files.append(os.path.join(root, file))

    print(f"Found {len(json_files)} JSON problem files.")

    # Build index mappings for JSON files
    json_by_slug = {}
    json_by_title_slug = {}
    for filepath in json_files:
        filename_slug = get_slug(os.path.splitext(os.path.basename(filepath))[0])
        json_by_slug[filename_slug] = filepath

        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                data = json.load(f)
                if 'title' in data:
                    title_slug = get_slug(data['title'])
                    json_by_title_slug[title_slug] = filepath
        except Exception as e:
            print(f"Warning: Could not parse {filepath}: {e}")

    # Read out.html
    print("Reading and parsing out.html...")
    with open(html_path, 'r', encoding='utf-8') as f:
        html_content = f.read()

    # Match all titles and descriptions
    # Matches <div id="title">...</div> followed by description content
    pattern = re.compile(
        r'<div id=\"title\">(.*?)</div>.*?<div class=\"HTMLContent_html__0OZLp\" data-track-load=\"description_content\">(.*?)</div>(?=<br><br><hr><br>|$)',
        re.DOTALL
    )
    matches = pattern.findall(html_content)
    print(f"Found {len(matches)} descriptions in out.html.")

    matched_count = 0
    skipped_count = 0
    written_html_count = 0

    for idx, (title, raw_desc) in enumerate(matches):
        slug = get_slug(title)
        
        # Compress the HTML content
        compressed_desc = compress_html(raw_desc)

        # 1. Write the HTML file to descriptions_html/
        html_filename = f"{slug}.html"
        html_filepath = os.path.join(output_html_dir, html_filename)
        with open(html_filepath, 'w', encoding='utf-8') as f:
            f.write(compressed_desc)
        written_html_count += 1

        # 2. Find the corresponding JSON file
        target_json = None
        # Try matching by title slug first
        if slug in json_by_title_slug:
            target_json = json_by_title_slug[slug]
        # Fallback to matching by filename slug
        elif slug in json_by_slug:
            target_json = json_by_slug[slug]
        # Double fallback: check if removing underscores matches (e.g. sqrt_x vs sqrtx)
        else:
            normalized_slug = slug.replace('_', '')
            for title_slug_key, path in json_by_title_slug.items():
                if title_slug_key.replace('_', '') == normalized_slug:
                    target_json = path
                    break
            if not target_json:
                for filename_slug_key, path in json_by_slug.items():
                    if filename_slug_key.replace('_', '') == normalized_slug:
                        target_json = path
                        break

        if target_json:
            # Load JSON, replace description with compressed HTML, delete redundant keys, and save
            try:
                with open(target_json, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                
                # Update description
                data['description'] = compressed_desc

                # Remove redundant keys
                for key in ["task", "inputFormat", "outputFormat", "constraints"]:
                    if key in data:
                        del data[key]
                
                with open(target_json, 'w', encoding='utf-8') as f:
                    json.dump(data, f, indent=2, ensure_ascii=False)
                
                matched_count += 1
            except Exception as e:
                print(f"Error updating {target_json}: {e}")
        else:
            skipped_count += 1

    print("\n===========================================")
    print(" Execution Summary")
    print("===========================================")
    print(f" HTML files written to {output_html_dir}: {written_html_count}")
    print(f" JSON files successfully updated       : {matched_count}")
    print(f" Descriptions in HTML not matched/skipped: {skipped_count}")
    print("===========================================")

if __name__ == '__main__':
    main()
