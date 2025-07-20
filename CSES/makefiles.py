#!/usr/bin/env python3
import os
import re
import requests
from bs4 import BeautifulSoup

BASE_URL = 'https://cses.fi/problemset/'
ROOT_DIR = '.'

def slugify(text):
    # Lowercase, replace non-alphanumeric with underscores
    text = text.lower()
    return re.sub(r'[^a-z0-9]+', '_', text).strip('_')

def fetch_problemset():
    resp = requests.get(BASE_URL)
    resp.raise_for_status()
    return BeautifulSoup(resp.text, 'html.parser')

def main():
    soup = fetch_problemset()
    # Ensure root directory exists
    os.makedirs(ROOT_DIR, exist_ok=True)
    print(f'Created root directory: {ROOT_DIR}')

    # Find categories and their problem lists
    for header in soup.find_all('h2'):
        category = header.text.strip()
        cat_dir = os.path.join(ROOT_DIR, slugify(category))
        os.makedirs(cat_dir, exist_ok=True)
        print(f'Created category directory: {cat_dir}')
        
        # The next sibling <ul> contains the list of problems
        ul = header.find_next_sibling('ul')
        if not ul:
            continue
        for li in ul.find_all('li'):
            # Extract title and link
            a = li.find('a')
            if not a:
                continue
            title = a.text.strip()
            filename = f'{slugify(title)}.cpp'
            filepath = os.path.join(cat_dir, filename)
            # Write boilerplate
            with open(filepath, 'w') as f:
                f.write('#include <bits/stdc++.h>\n')
                f.write('using namespace std;\n\n')
                f.write('int main() {\n')
                f.write('    ios::sync_with_stdio(false);\n')
                f.write('    cin.tie(nullptr);\n\n')
                f.write('    // TODO: solve "' + title + '"\n\n')
                f.write('    return 0;\n')
                f.write('}\n')
            print(f'Created file: {filepath}')

if __name__ == '__main__':
    main()
