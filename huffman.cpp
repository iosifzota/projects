#include <sstream>
#include <memory>
#include <iostream>
#include <functional>
#include <stack>
#include <utility>

using std::make_shared;
using std::string;
using std::greater;
using std::stack;

#include "freq.hh"
#include "req.hh"

#include "priority_queue.hh"

namespace std {
    template <>
    struct greater<sptr<hchar_t>>
    {
        bool operator() (const sptr<hchar_t>& lhs, const sptr<hchar_t>& rhs) {
            return lhs->freq > rhs->freq;
        }
    };
}
using min_heap = priority_queue< sptr<hchar_t>, greater<sptr<hchar_t>> >;

namespace huffman {
    class huff_tree
    {
        private:
            sptr<hchar_t> root;
        public:
            huff_tree(min_heap&);
            huff_tree(const char *file_path);

            vec<string> create_codes();
            void get_codes(vec<string>& codes) const;

            string decode(string) const;
    };

    min_heap get_nodes(const char* file_path);
    static inline min_heap get_nodes(const vec<size_t>& freq);

    void create_tree(sptr<hchar_t>& root, const char *file_path);
    static inline void create_tree(sptr<hchar_t>& root, min_heap&);

    string encode(const char* file_path, const vec<string>& codes);
}

using huffman::huff_tree;
using huffman::encode;

constexpr char input_file[] = "../input/huff.txt";

int test_huffman()
{
        std::cout << input_file << '\n';
        huff_tree hf(input_file);

        vec<string> codes = hf.create_codes();

        for (size_t c = 0; c < codes.size(); ++c)
                if (!codes[c].empty())
                        std::cout << c << ": " << codes[c] << "\n\n";

        string encoding = encode(input_file, codes);
        std::cout << "Encoded:\n" << encoding; 	nn(2);

        string decoding = hf.decode(encoding);
        std::cout << "Decoded:\n" << decoding; 	nn(2);

        return 0;
}

int main() {
        test_huffman();
}


namespace huffman {

    string huff_tree::decode(string encoding) const
    {
        string decoding;
        sptr<hchar_t> itr;
        size_t pos;

        req(root != nullptr, "Null tree.");

        pos = 0;
        while (pos < encoding.size()) {

            itr = root;
            req(encoding[pos++] == '0', "Every char code must start with 0.");

            /* 0 = take left; 1 = take right */
            while (pos < encoding.size() && itr->c == 0) {
                if (encoding[pos] == '0') {
                    itr = itr->left;
                }
                else if (encoding[pos] == '1') {
                    itr = itr->right;
                }
                else {
                    std::cerr << "[Error] Unexpected symbol: " << encoding[pos] << std::endl;
                    exit(-1);
                }

                req(itr != nullptr, "Undefined char code. [Debug]");
                ++pos;
            }

            req(itr->c != 0, "Read incomplete char code.");

            decoding.push_back(itr->c);
        }

        return decoding;
    }

    vec<string> huff_tree::create_codes()
    {
        constexpr size_t size = MAX_UTF16;
        vec<string> codes(size);

        get_codes(codes);
        return codes;
    }

    huff_tree::huff_tree(const char *file_path)
    {
        create_tree(root, file_path);
    }

    huff_tree::huff_tree(min_heap& p)
    {
        create_tree(root, p);
    }

    /* Helpers */
    min_heap get_nodes(const vec<size_t>& freq)
    {
        min_heap hchar_nodes;

        auto size = freq.size();
        req(size <= MAX_UTF16 + 1, "freq.size exceeds max number of uft16 chars.");

        for (size_t c = 0; c < size; ++c) {
            req(c == (utf16)c, "Overflowed utf16. [Debug]");

            if (freq[c]) {
                hchar_nodes.push(make_shared<hchar_t>((utf16)c, freq[c]));
            }
        }

        return hchar_nodes;
    }

    min_heap get_nodes(const char* file_path)
    {
        ifstream in(file_path);

        req(in.good(), "Null input.");

        vec<size_t> freq;
        min_heap temp, hchar_nodes;

        freq = find_freq(in); /*and*/ in.close();
        hchar_nodes = get_nodes(freq);

        return hchar_nodes;
    }

    void create_tree(sptr<hchar_t>& root, const char *file_path)
    {
        min_heap data = get_nodes(file_path);

        /* Print nodes */
        std::cout << "Printing...\n";
        data.map([](const sptr<hchar_t>& ptr) {
                std::cout << "'" << ptr->c << "'" << ": " << ptr->freq << '\t';
                });

        create_tree(root, data);
    }

    void create_tree(sptr<hchar_t>& root, min_heap& data)
    {
        sptr<hchar_t> left_child, right_child, new_node;

        req(!data.empty(), "Empty queue.");

        while (!data.empty()) {
            right_child = data.top(); /*and*/ data.pop();

            /* Leave after popping the last element. */
            if (data.empty()) {
                break;
            }

            left_child = data.top(); /*and*/ data.pop();

            new_node = std::make_shared<hchar_t>(
                    (utf16)0,
                    right_child->freq + left_child->freq,
                    left_child,
                    right_child
                    );

            data.push(new_node);
        }

        root = right_child;
        req(root != nullptr, "Nullptr root. [Debug]");
    }

    enum binary_char {
        ZERO = '0',
        ONE = '1'
    };

    #define POST_EXEC nullptr
    constexpr char NIL_CHAR = 0;

    static inline bool is_leaf(const sptr<hchar_t>& node)
    {
        req(node != nullptr);

        if (node->c == NIL_CHAR) {
            return false;
        }

        req( (node->right == nullptr) && (node->left == nullptr) );
        return true;
    }

    void huff_tree::get_codes(vec<string>& codes) const
    {
        sptr<hchar_t> node;
        string code;

        /* simulate stack call */
        stack<sptr<hchar_t>> st;
        stack<utf8> bits;

        req(root != nullptr, "[Debug]");

        st.push(root);
        bits.push(ZERO);

        while (!st.empty()) {
            auto node = st.top(); /**/ st.pop();

            if (node == POST_EXEC) {
                code.pop_back();
                continue;
            }

            code.push_back(bits.top()); /**/ bits.pop();

            if (is_leaf(node)) {
                req(node->c < codes.size(), "Bounds check.");
                codes[node->c] = code;

                code.pop_back();
            }
            else {
                st.push(POST_EXEC);

                if (node->right != nullptr) {
                    st.push(node->right);
                    bits.push(ONE);
                }

                if (node->left != nullptr) {
                    st.push(node->left);
                    bits.push(ZERO);
                }
            }
        }
    }

    string encode(const char* file_path, const vec<string>& codes)
    {
        ifstream in(file_path);
        string encoding;
        utf16 c;

        while (next_char(in, c)) {
            req(c < codes.size(), "Bounds check failed.");

            // TODO: concat strings rather then appending char by char.
            for (const auto& c_index : codes[c]) {
                encoding.push_back(c_index);
            }
        }
        std::cout << '\n';

        return encoding;
    }

}
