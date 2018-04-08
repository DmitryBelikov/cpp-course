#include "archiever.h"
#include <set>
#include <cassert>
#include <iostream>
#include <memory>

node:: node():
    value(0),
    son{nullptr, nullptr}
{}

node:: node(uint32_t value):
    value(value),
    son{nullptr, nullptr}
{}

node:: ~node()
{

}

uint32_t node::get_value()
{
    return value;
}

bool node::is_leaf()
{
    return son[0] == nullptr;
}

void node::get_codes(std::vector <uint64_t> &result, std::vector<uint32_t> &sz, uint64_t trace, uint32_t h)
{
    if(is_leaf())
    {
        result[value] = trace;
        sz[value] = h;
        return;
    }

    son[0]->get_codes(result, sz, trace, h + 1);
    son[1]->get_codes(result, sz, trace + (1 << h), h + 1);
}

huffman:: huffman():
    root(new node),
    current(root.get())
{}

huffman:: ~huffman()
{

}

void huffman::build(const std::vector <uint64_t> &cnt)
{
    using namespace std;
    struct tree_node {
        uint64_t occurence;
        uint32_t height;
        node* ptr;

        bool operator <(const tree_node& other) const {
            if(occurence != other.occurence)
                return occurence < other.occurence;
            if(height != other.height)
                return height < other.height;
            return ptr < other.ptr;
        }
    };

    set<tree_node> s;
    for(size_t i = 0; i != 256; ++i)
        s.insert({cnt[i], 0, new node(i)});
    while(s.size() > 1)
    {
        auto it = s.begin();
        auto p = *it;
        s.erase(it);

        it = s.begin();
        auto q = *it;
        s.erase(it);

        node* to_add = new node;

        if(p.height > q.height)
        {
            to_add->son[0].reset(p.ptr);
            to_add->son[1].reset(q.ptr);
        } else
        {
            to_add->son[0].reset(q.ptr);
            to_add->son[1].reset(p.ptr);
        }
        s.insert({p.occurence + q.occurence, max(p.height, q.height) + 1, to_add});
    }
    if(s.begin()->height > 50)
        throw std::runtime_error("This file wasn't encoded with this archiever");
    root.reset(s.begin()->ptr);
    current = root.get();
}

std::pair <std::vector <uint64_t>, std::vector <uint32_t> > huffman::get_codes()
{
    std::vector <uint64_t> result(256);
    std::vector <uint32_t> sz(256);
    root->get_codes(result, sz, 0, 0);
    return {result, sz};
}

uint32_t huffman::make_step(uint32_t x)
{
    current = current->son[x].get();
    if(current->is_leaf())
    {
        uint32_t res = current->get_value();
        current = root.get();
        return res;
    }
    return NO_ANSWER;
}

archiever::archiever():
    buf_size(0),
    buffer(0),
    code(256),
    sz(256),
    cnt(256)
{}

archiever::archiever(std::vector <uint64_t> data):
    buf_size(0),
    buffer(0),
    code(256),
    sz(256),
    cnt(data)
{}

archiever::~archiever()
{

}

std::string archiever::get_info()
{
    std::string res;
    for(size_t i = 0; i != 256; ++i)
        res += std::to_string(cnt[i]) + "\n";
    return res;
}

void archiever::update(const char *a, size_t sz) {
    for(size_t i = 0; i != sz; ++i)
        ++cnt[(uint8_t)(a[i])];
    is_builded = false;
}

void archiever::encode(const char *a, size_t string_sz, std::vector <char> &to_write)
{
    using namespace std;
    if(!is_builded)
    {
        is_builded = true;
        tree.build(cnt);
        pair <vector <uint64_t>, vector<uint32_t> > p = tree.get_codes();
        code = p.first;
        sz = p.second;
    }
    for(size_t i = 0; i != string_sz; ++i)
    {
        uint64_t v = code[(uint8_t)a[i]];
        buffer += (v << buf_size);
        buf_size += sz[(uint8_t)a[i]];
        while(buf_size >= 8)
        {
            buf_size -= 8;
            to_write.push_back((char)(buffer & 255));
            buffer >>= 8;
        }
    }
}

void archiever::decode(const char *a, size_t string_sz, std::vector <char> &to_write)
{
    using namespace std;
    if(!is_builded)
    {
        is_builded = true;
        tree.build(cnt);
        pair <vector <uint64_t>, vector<uint32_t> > p = tree.get_codes();
        code = p.first;
        sz = p.second;
    }
    for(size_t i = 0; i != string_sz; ++i)
    {
        uint8_t x = (uint8_t)a[i];
        for(size_t j = 0; j != 8; ++j)
        {
            uint32_t answer = tree.make_step((x >> j) & 1);
            if(answer == tree.NO_ANSWER)
                continue;
            to_write.push_back((char)answer);
        }
    }
}


bool archiever::end()
{
    return buf_size == 0;
}

char archiever::get_last()
{
    return (char)buffer;
}

unsigned long long my_stoll(std::string s)
{
    size_t sz;
    unsigned long long value = std::stoull(s, &sz);
    if(sz != s.size())
        throw std::invalid_argument("This file wasn't encoded with this archiever");
    return value;
}
