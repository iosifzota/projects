#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "hash_table.hh"
#include "utils.hh"
#include "req.hh"
#include "hashes.hh"

unsigned test_add_hash(const char*);
unsigned test_xor_hash(const char*);
unsigned test_rot_hash(const char*);
unsigned test_djb_hash(const char*);
unsigned test_std_hash(const char*);
unsigned test_fnv_hash(const char*);

void test_story();
void test_set();

int main()
{
        test_set();
        test_story();
        return 0;
}

#if  defined (__GNUC__) || defined(__GNUG__)
#define STRINGS_SET_FILE "../input/rand_strings.txt"
#define STORY_FILE "../input/alice.txt"
#else
#define STRINGS_SET_FILE "Text.txt"
#define STORY_FILE "Story.txt"
#endif

void test_set()
{
        test_wrap("Set",
                  std::vector<Hash_Stats> data;

                  data.push_back({ "Add_Hash", test_add_hash(STRINGS_SET_FILE) });
                  data.push_back({ "Xor_Hash", test_xor_hash(STRINGS_SET_FILE) });
                  data.push_back({ "Rot_Hash", test_rot_hash(STRINGS_SET_FILE) });
                  data.push_back({ "Djb_Hash", test_djb_hash(STRINGS_SET_FILE) });
                  data.push_back({ "Std_Hash", test_std_hash(STRINGS_SET_FILE) });
                  data.push_back({ "FNV_Hash", test_fnv_hash(STRINGS_SET_FILE) });

                  std::cout << "\nStats:\n";
                  std::sort(data.begin(), data.end());
                  for (const auto& hash : data) {
                          std::cout << hash; nl;
                  }
                );
}

void test_story()
{
        test_wrap("Story",
                  std::vector<Hash_Stats> data;

                  data.push_back({ "Add_Hash", test_add_hash(STORY_FILE) });
                  data.push_back({ "Xor_Hash", test_xor_hash(STORY_FILE) });
                  data.push_back({ "Rot_Hash", test_rot_hash(STORY_FILE) });
                  data.push_back({ "Djb_Hash", test_djb_hash(STORY_FILE) });
                  data.push_back({ "Std_Hash", test_std_hash(STORY_FILE) });
                  data.push_back({ "FNV_Hash", test_fnv_hash(STORY_FILE) });

                  std::cout << "\nStats:\n";
                  std::sort(data.begin(), data.end());
                  for (const auto& hash : data) {
                          std::cout << hash; nl;
                  }
                );
}

#define generate_test_hash_body(Hash_Name)                      \
        iz::htable< std::string, int, Hash_Name > test(15000U); \
                                                                \
        std::string word;                                       \
                                                                \
        std::ifstream input(file_path);                         \
        req(input.good(), "Error opening file.");               \
                                                                \
        unsigned i = 0;                                         \
        for (;input >> word; ++i) {                             \
                test.insert(word, i);                           \
        }                                                       \
                                                                \
        std::cout << "Inserted: " << i << ". "                  \
        << "Unique words count: "<< test.count() << '\n';       \
        input.close();

unsigned test_add_hash(const char* file_path)
{
        generate_test_hash_body(Add_Hash);
        return test.collision_count();
}

unsigned test_xor_hash(const char* file_path)
{
        generate_test_hash_body(Xor_Hash);
        return test.collision_count();
}

unsigned test_rot_hash(const char* file_path)
{
        generate_test_hash_body(Rot_Hash);
        return test.collision_count();
}

unsigned test_djb_hash(const char* file_path)
{
        generate_test_hash_body(Djb_Hash);
        return test.collision_count();
}

unsigned test_std_hash(const char* file_path)
{
        generate_test_hash_body(std::hash<std::string>);
        return test.collision_count();
}
unsigned test_fnv_hash(const char* file_path)
{
        generate_test_hash_body(FNV_Hash);
        return test.collision_count();
}

#undef INPUT_FILE
