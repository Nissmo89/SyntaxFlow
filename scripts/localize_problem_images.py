#!/usr/bin/env python3
"""Download remote images referenced in problem description HTML and replace
the remote URLs with local file paths so the app works fully offline.

Filenames are the hex encoding of the full image URL, which guarantees a unique,
conflict-free name regardless of the original filename or path.
"""
import os
import re
import json
import sys
import hashlib
import urllib.request
import urllib.error

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PROBLEMS_DIR = os.path.join(REPO_ROOT, "problems")
ASSETS_DIR = os.path.join(REPO_ROOT, "web_editor", "assets", "problem_images")
HTML_DIR = os.path.join(PROBLEMS_DIR, "descriptions_html")

SRC_RE = re.compile(r'(src\s*=\s*["\'])(https?://[^"\'>\s]+)(["\'])')
IMG_ATTR_RE = re.compile(r'(https?://[^"\'>\s]+\.(?:png|jpe?g|gif|webp|svg|bmp|ico|avif))', re.IGNORECASE)

def url_to_hex(url):
    return hashlib.sha256(url.encode('utf-8')).hexdigest()

def extension_for_url(url, content_type):
    ct = (content_type or '').split(';')[0].strip().lower()
    mapping = {
        'image/png': '.png',
        'image/jpeg': '.jpg',
        'image/jpg': '.jpg',
        'image/gif': '.gif',
        'image/webp': '.webp',
        'image/svg+xml': '.svg',
        'image/bmp': '.bmp',
        'image/x-icon': '.ico',
        'image/vnd.microsoft.icon': '.ico',
        'image/avif': '.avif',
    }
    if ct in mapping:
        return mapping[ct]
    m = re.search(r'\.([a-zA-Z0-9]+)(?:[?#]|$)', url)
    if m and m.group(1).lower() in {'png', 'jpg', 'jpeg', 'gif', 'webp', 'svg', 'bmp', 'ico', 'avif'}:
        return '.' + m.group(1).lower()
    return '.img'

def download(url, dest_base):
    """Download url to dest_base (no extension). Returns (success, msg, filename)."""
    ext = None
    if os.path.exists(dest_base):
        for f in os.listdir(os.path.dirname(dest_base)):
            if f.startswith(os.path.basename(dest_base)):
                return True, "already exists", f
    req = urllib.request.Request(url, headers={
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) SyntaxFlow-offline-images/1.0',
        'Accept': 'image/*,*/*;q=0.8',
    })
    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            data = resp.read()
            content_type = resp.headers.get('Content-Type', '')
            if not data:
                return False, "empty response", None
            ext = extension_for_url(url, content_type)
            final = dest_base + ext
            os.makedirs(os.path.dirname(final), exist_ok=True)
            with open(final, 'wb') as f:
                f.write(data)
            return True, f"downloaded ({ext})", os.path.basename(final)
    except Exception as e:
        return False, str(e), None

def rewrite_description(html, url_map):
    """Replace remote image src URLs with local asset paths."""
    def repl(m):
        url = m.group(2)
        if url in url_map:
            return m.group(1) + url_map[url] + m.group(3)
        return m.group(0)
    return SRC_RE.sub(repl, html)

def main():
    os.makedirs(ASSETS_DIR, exist_ok=True)

    json_files = []
    for root, _, files in os.walk(PROBLEMS_DIR):
        if 'descriptions_html' in root:
            continue
        for name in files:
            if name.endswith('.json'):
                json_files.append(os.path.join(root, name))

    all_urls = set()
    json_desc = {}
    for path in json_files:
        try:
            with open(path, 'r', encoding='utf-8') as f:
                data = json.load(f)
        except Exception as e:
            print(f"  WARN skip {path}: {e}")
            continue
        desc = data.get('description', '') or ''
        urls = set(IMG_ATTR_RE.findall(desc))
        all_urls |= urls
        json_desc[path] = desc

    html_files = [os.path.join(HTML_DIR, f) for f in os.listdir(HTML_DIR) if f.endswith('.html')] if os.path.isdir(HTML_DIR) else []
    html_content = {}
    for path in html_files:
        try:
            with open(path, 'r', encoding='utf-8') as f:
                html_content[path] = f.read()
        except Exception as e:
            print(f"  WARN skip {path}: {e}")

    for content in html_content.values():
        all_urls |= set(IMG_ATTR_RE.findall(content))

    all_urls = sorted(all_urls)
    print(f"Found {len(all_urls)} unique remote image URLs across {len(json_files)} JSON files and {len(html_files)} HTML files.\n")

    url_map = {}
    ok = fail = 0
    for url in all_urls:
        name = url_to_hex(url)
        dest_base = os.path.join(ASSETS_DIR, name)
        success, msg, fname = download(url, dest_base)
        if success:
            rel = os.path.join('assets', 'problem_images', fname).replace('\\', '/')
            url_map[url] = rel
            ok += 1
            print(f"  OK   {url}")
        else:
            fail += 1
            print(f"  FAIL {url}  ({msg})")

    print(f"\nDownloaded {ok}, failed {fail}.")

    if fail:
        print(f"{fail} image(s) failed to download; those URLs will be left untouched in the HTML.")

    updated_json = 0
    for path, desc in json_desc.items():
        new_desc = rewrite_description(desc, url_map)
        if new_desc != desc:
            try:
                with open(path, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                data['description'] = new_desc
                with open(path, 'w', encoding='utf-8') as f:
                    json.dump(data, f, indent=2, ensure_ascii=False)
                updated_json += 1
            except Exception as e:
                print(f"  WARN failed to update {path}: {e}")

    updated_html = 0
    for path, content in html_content.items():
        new_content = rewrite_description(content, url_map)
        if new_content != content:
            with open(path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            updated_html += 1

    print(f"Updated {updated_json} JSON files and {updated_html} HTML files.")

    remaining = 0
    for path, desc in json_desc.items():
        with open(path, 'r', encoding='utf-8') as f:
            current = json.load(f).get('description', '') or ''
        if SRC_RE.search(current):
            remaining += 1
    print(f"JSON files still containing remote image src: {remaining}")

if __name__ == '__main__':
    main()