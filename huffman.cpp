#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <iostream>

#include "freq.hh"
#include "utils.hh"
#include "req.hh"

#include "priority_queue.hh"

/* Used for comparison. => Will result in a min-queue. */
struct Greater {
        bool operator() (const std::shared_ptr<h_char>& lhs, const std::shared_ptr<h_char>& rhs) {
                return lhs->freq > rhs->freq;
        }
};

/* Given a file_path returns a vector of h_char nodes. */
priority_queue< std::shared_ptr<h_char>, Greater> read_file(const char* file_path);

/* Create h_char vector from frequencies vector. */
priority_queue< std::shared_ptr<h_char>, Greater> create_freqs(const std::vector<unsigned>& freq);

class huff_tree
{
public:

        /* Create tree from an ordered h_char vector. */
        void create_tree(priority_queue< std::shared_ptr<h_char>, Greater>&);
        void create_tree_from_file(const char *file_path);

        std::string decode(std::string);

        std::vector<std::string> create_codes();
        void cc_body(
                std::vector <std::string>& codes,
                const std::shared_ptr<h_char>& current_node,
                char cc, std::string& code);

private:
        std::shared_ptr<h_char> root;
};

std::string encode(const char* file_path, const std::vector<std::string>& codes);

int test_huffman();

int main() {
        test_huffman();
}

#define input_file_name "huff.txt"
#define input_dir "../input/"

#if  defined (__GNUC__) || defined(__GNUG__)
      #define input_file input_dir input_file_name
#else
      #define input_file input_file_name
#endif

int test_huffman()
{
        huff_tree hf;

        std::cout << input_file << '\n';

        /* Create huff_tree. */
        hf.create_tree_from_file(input_file);

        /* Create vector of prefix codes for the characters. */
        std::vector<std::string> codes = hf.create_codes();

        /* Print out the code for each character. */
        for (unsigned char c = 0; c < codes.size(); ++c)
                if (!codes[c].empty())
                        std::cout << c << ": " << codes[c] << "\n\n";

        /* Encode the text. */
        std::string encoding = encode(input_file, codes);
        std::cout << "Encoded:\n" << encoding; 	nn(2);

        /* Decode the text. */
        std::string decoding = hf.decode(encoding);
        std::cout << "Decoded:\n" << decoding; 	nn(2);

        return 0;
}

std::string huff_tree::decode(std::string encoding)
{
        std::string decoding;
        std::shared_ptr<h_char> itr;

        unsigned pos = 0;

        req(root != nullptr, "Null tree.");

        while (pos < encoding.size()) {

                itr = root;
                req(encoding[pos++] == '0', "Every char code must start with 0.");

                /* Follow the bits left and right through the tree. */
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

std::string encode(const char* file_path, const std::vector<std::string>& codes)
{
        std::ifstream in(file_path);

        std::string encoding;

        int c;

        while ((c = in.get()) != EOF) {
		req( (c) < (long)codes.size(), "Tring to access vector out of bounds.[Debug]");

                for (const auto& c_index : codes[c]) {
                        encoding.push_back(c_index);
                }
        }
	nl;

	return encoding;
}


void huff_tree::create_tree(priority_queue< std::shared_ptr<h_char>, Greater>& data)
{
        std::shared_ptr<h_char> left_child, right_child, new_node;

        req(!data.empty(), "Empty queue.");

        while (!data.empty()) {
                right_child = data.top();
                data.pop();

                /* Will be proced when there's only one element. */
                if (data.empty()) {
                        break;
                }

                left_child = data.top();
                data.pop();

                /* Create new parent. */
                new_node = std::make_shared<h_char>(
                        (char)0,
                        right_child->freq + left_child->freq,
                        left_child,
                        right_child
                        );

                /* Insert the new parent. */
                data.push(new_node); // insert(data, new_node)
        }

        root = right_child;
        req(root != nullptr, "Nullptr output.");
}


priority_queue< std::shared_ptr<h_char>, Greater> create_freqs(const std::vector<unsigned>& freq)
{
        std::shared_ptr<h_char> node;
        // std::vector< std::shared_ptr<h_char> > data;

        priority_queue< std::shared_ptr<h_char>, Greater> d;

        for (unsigned c = 0; c < freq.size(); ++c) {
                if (freq[c]) {
                        node = std::make_shared<h_char>((char)c, freq[c]);

                        d.push(node);
                }
        }

        return d;
}


priority_queue< std::shared_ptr<h_char>, Greater> read_file(const char* file_path)
{
        std::ifstream in(file_path);

	req(in, "Null input.");

        std::vector<unsigned> freq;
        priority_queue< std::shared_ptr<h_char>, Greater > data;

        freq = find_freq(in);
        in.close();

        /* Create h_char vector from frequencies vector. */
        priority_queue< std::shared_ptr<h_char>, Greater> d;
        d = create_freqs(freq);

        data = d;
        while (!data.empty()) {
                std::shared_ptr<h_char> ptr;
                ptr = data.top();
                data.pop();

                std::cout << "'" << ptr->c << "'" << ": " << ptr->freq << '\t';
        }
        nll;

        return d;
}


std::vector<std::string> huff_tree::create_codes()
{
        std::vector<std::string> codes(200);
        std::string dummy;

        cc_body(codes, root, '0', dummy);

        return codes;
}

void huff_tree::cc_body(
        std::vector <std::string>& codes,
        const std::shared_ptr<h_char>& current_node,
        char cc, std::string& code)
{
        req(current_node != nullptr, "Null input. [Debug]");

        code.push_back(cc);

        if (current_node->c != 0) {
                /* Require characters to be leaf nodes. */
                req(current_node->right == nullptr);
                req(current_node->left  == nullptr);

                /* Reached leaf node => generated code for the current character. */
                codes[current_node->c] = code;
        }

        if (current_node->left != nullptr) {
                cc_body(codes, current_node->left, '0', code);
        }

        if (current_node->right != nullptr) {
                cc_body(codes, current_node->right, '1', code);
        }

        code.pop_back();
}

void huff_tree::create_tree_from_file(const char *file_path)
{
        priority_queue< std::shared_ptr<h_char>, Greater> data = read_file(file_path);
        create_tree(data);
}
