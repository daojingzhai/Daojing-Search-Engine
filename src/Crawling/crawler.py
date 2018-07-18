
'''
This is a crawler for cs data structure course project.
The crawel achieves two main function: crawl web and chinese segmentation.
Output is stored into folder: \data, including \content, \html, \vocab, and database.txt
Seed page is CSDN main page,
Sub pages are constrained in CSDN blog.
Use newpaper3k to get clean text from heml and jieba segment vocab
'''

import re
import sys
import requests
import os
from collections import deque
import newspaper
from zhon.hanzi import punctuation
import jieba
from decimal import Decimal

num_pages = 10  # Page numbers
width = 5  # Page width
data_path = os.path.abspath('..')


''' Crawl web'''


# Get source code of a page
def get_source(url):
    response = requests.get(url, timeout=60)
    if response.status_code != requests.codes.OK:
        return "", 0
    if "text/html" not in response.headers["content-type"]:
        return "", 0
    source = response.text
    return source, 1


# append hyperlinks to queue
def queue_append(done, queue, links):
    for i in links:
        if i not in queue and i not in done:
            queue.append(i)


# add links to history
def add_history(history, links):
    for i in links:
        if i not in history:
            history.append(i)



# Get all links in a page
def get_links(history, page):
    source, state = get_source(page)
    if state == 0:
        if page in history:
            history.remove(page)
        print("Invalid URL")
        raise Exception("Invalid URL")
    lists = []
    links = re.findall('"((https|ftp)://blog.csdn.net/.*?/article/details/\d*)"'
                       , source)
    source, state = get_source(page)
    if state == 0:
        if page in history:
            history.remove(page)
        print("Invalid URL")
        raise Exception("Invalid URL")
    lists = []
    links = re.findall('"((https|ftp)://blog.csdn.net/.*?/article/details/\d*)"', source)
    for i in links:
        if i[0] != page: #avoid self line
            lists.append(i[0])
    lists = list(set(lists)) #avoid repeat

    return source, lists


# write the crawed page to database.txt
def write_database(history, page, links):
    try:
        with open(data_path+"/Data/database.txt", 'a') as file:
            print(str(history.index(page)), page)
            file.write(str(history.index(page)))
            file.write(":")
            file.write(page)
            for i in links:
                if history.index(i) < num_pages:
                    file.write(' ')
                    file.write(str(history.index(i)))
            file.write('\n')
    except:
        raise Exception("Database File Fail to Load")

# Record html sorce
def write_source(history, page, source):
    try:
        with open(data_path+"/Data/html/"+str(history.index(page))+".txt", 'w') as file:
            file.write(source)
    except:
        raise Exception("Html File Fail to Load")


# Web mining
def crawl_web(seed_url):
    queue = deque() # Data Structure: queue
    history = []
    queue.append(seed_url)
    history.append(seed_url)
    done = [] # done web
    current_page = ""

    while queue:
        try:
            if len(done) > num_pages: #collect 100 pages total
                break
            page = queue.popleft()
            current_page = page
            done.append(page)
            source, links = get_links(history, page)
            if len(links) > width:
                links = links[0:width]
            add_history(history, links)
            queue_append(done, queue, links)
            write_database(history, page, links)
            write_source(history, page, source)
        except KeyboardInterrupt:
            sys.exit(0)
        except:
            done.pop()
            if current_page in history:
                history.remove(current_page)
            continue


''' Chinese Segmentation'''


# get rid of punctuation from text
def clean_text(text):
    text = re.sub("[%s]+" % punctuation, "", text)  # remove chinese punctuation
    text = re.sub(r'[^\w\s]', '', text)  # remove english punctuation

    return text

# get rid of ' ' and '\n' from vocab lists
def clean_lists(lists):
    while ' ' in lists:
        lists.remove(' ')
    while '\n' in lists:
        lists.remove('\n')

    return lists


# combine title and text
def lists_append(title, text):
    vocab = []
    for i in title:
        vocab.append(i)
    for j in text:
        if j not in vocab:
            vocab.append(j)

    return vocab


# get vocab lists for one page
def get_vocab_lists(i):
    try:
        with open(data_path+"/Data/html/" + str(i) + ".txt") as file:
            html = file.read()
            article = newspaper.Article('')
            article.set_html(html)
            article.parse()
            original_title = article.title
            original_text = article.text
            title = clean_text(original_title)
            text = clean_text(original_text)
            vocab_title = list(jieba.cut_for_search(title))
            vocab_text = list(jieba.cut_for_search(text))
            vocab_title = clean_lists(vocab_title)
            vocab_text = clean_lists(vocab_text)
            vocab = lists_append(vocab_title, vocab_text)
    except KeyboardInterrupt:
        sys.exit(0)
    except:
        vocab = []
        original_title = ""
        original_text = ""
        print("Vocab Fail to Create")  # test

    return vocab, original_title, original_text


# get vocab lists for all pages in the database
def write_vocab_lists():
    cnt = 0
    num = num_pages+1
    for i in range(num):
        vocab, title, text = get_vocab_lists(i)
        with open(data_path+"/Data/content/" + str(cnt) + ".txt", 'w') as file:
            file.write(title)
            file.write('\n\n')
            file.write(text)
        with open(data_path+"/Data/vocab/" + str(cnt) + ".txt", 'w') as file:
            for i in vocab:
                file.write(i)
                file.write('\n')
        if cnt % 10 == 0:
            print(str(round(Decimal(cnt / num_pages * 100), 1)) + '%')
        cnt += 1



# main function
def main():
    #seed url
    crawl_web("https://www.csdn.net/nav/db")
    print("Crawling Done, Next Step: Segmentation")
    #segmentation
    print("writing vocab lists...")
    write_vocab_lists()
    print("Segmentation Done.")


# entry
if __name__ == '__main__':
    main()
