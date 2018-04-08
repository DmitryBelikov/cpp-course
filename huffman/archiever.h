#include <string>
#include <vector>
#include <memory>

struct node
{
private:
    uint32_t value;
public:
    node();
    node(uint32_t);
    ~node();

    std::unique_ptr<node> son[2];
    bool is_leaf();
    uint32_t get_value();
    void get_codes(std::vector <uint64_t> &, std::vector <uint32_t> &, uint64_t, uint32_t);
};

struct huffman
{
private:
    std::unique_ptr<node> root;
    node* current;
public:
    huffman();
    ~huffman();

    void build(const std::vector <uint64_t> &);
    std::pair <std::vector <uint64_t>, std::vector <uint32_t> > get_codes();

    uint32_t make_step(uint32_t);

    static const uint32_t NO_ANSWER = 10000;

};

struct archiever
{
private:
    void build();

    int32_t buf_size;
    uint32_t buffer;
    std::vector <uint64_t> code;
    std::vector <uint32_t> sz;
    std::vector <uint64_t> cnt;
    huffman tree;

    bool is_builded = false;
public:
    archiever();
    archiever(std::vector <uint64_t>);
    ~archiever();

    void update(const char *, size_t);
    void encode(const char *, size_t, std::vector <char> &);
    void decode(const char *, size_t, std::vector <char> &);
    std::string get_info();

    bool end();
    char get_last();
};

unsigned long long my_stoll(std::string);
