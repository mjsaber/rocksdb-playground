import urllib
import json
import xml.etree.ElementTree as ET

BASE_URL = "http://export.arxiv.org/api/query?search_query=all&start=%d&max_results=%d"

def parse_entry(root):
    if not root.tag.endswith("entry"):
        return None
    title = ""
    authors = []
    for child in root:
        if child.tag.endswith("title"):
            title = child.text
        elif child.tag.endswith("author"):
            for author_child in child:
                if author_child.tag.endswith("name"):
                    authors.append(author_child.text)
    return {"title": title, "citation": ", ".join(authors)}

def crawl_arxiv(start, size):
    print("crawling start=%d, size=%d..." % (start, size))
    data = urllib.urlopen(BASE_URL % (start, size)).read()
    root = ET.fromstring(data)
    res = []
    for child in root:
        cur = parse_entry(child)
        if cur:
            res.append(cur)
    return res

def crawl_all(size):
    cur_idx = 1
    cur_start = 0
    batch_size = 30
    title_map = {}
    res = []
    while cur_idx <= size:
        cur_res = crawl_arxiv(cur_start, batch_size)
        for m in cur_res:
            if m["title"] in title_map:
                continue
            m["id"] = cur_idx
            title_map[m["title"]] = cur_idx
            cur_idx = cur_idx + 1
            res.append(json.dumps(m))
        if len(cur_res) < batch_size:
            break
        cur_start = cur_start + len(cur_res)
    return res


if __name__ == "__main__":
    print("Begin to crawl arxiv...")
    output_path = "/tmp/data.json"
    total_cnt = 1000
    res = crawl_all(total_cnt)
    with open(output_path, "w") as fp:
        for ele in res:
            fp.write(ele + "\n")


