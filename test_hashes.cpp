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

void test_scenario(const char* scenario_name, const char* input_file);

#if  defined (__GNUC__) || defined(__GNUG__)
#define STRINGS_SET_FILE "../input/rand_strings.txt"
#define STORY_FILE "../input/alice.txt"
#else
#define STRINGS_SET_FILE "Text.txt"
#define STORY_FILE "Story.txt"
#endif

int main(int argc, char *argv[])
{
        test_scenario("Set", STRINGS_SET_FILE);
        test_scenario("Story", STORY_FILE);

        if (argc > 1) {
            std::cout << "More tests...\n";
        }

        for (int i = 1; i < argc; ++i) {
            const char* scenario_name = argv[i];

            i += 1;
            req(i < argc, "Usage: test_hash [scenario_name file_name]");
            test_scenario(scenario_name, argv[i]);
        }

        return 0;
}

void test_scenario(const char* scenario_name, const char* input_file)
{
        test_wrap(scenario_name,
                  std::vector<Hash_Stats> data;

                  data.push_back({ "Add_Hash", test_add_hash(input_file) });
                  data.push_back({ "Xor_Hash", test_xor_hash(input_file) });
                  data.push_back({ "Rot_Hash", test_rot_hash(input_file) });
                  data.push_back({ "Djb_Hash", test_djb_hash(input_file) });
                  data.push_back({ "Std_Hash", test_std_hash(input_file) });
                  data.push_back({ "FNV_Hash", test_fnv_hash(input_file) });

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
