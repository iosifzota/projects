#ifndef __hash_table_hh
#define __hash_table_hh

/*
   hash< Key, Val, Hash >

container:
    std::vector< std::list<Val> >;

methods:
    insert(Key, Val) ~~~ hash[Key] = Val    // inserts only unique Keys
    delete(Key)
    delete_objs_also(Key)
    delete_arrays_also(Key)

    search(Key) => iterator ... hash[Key] => val

iterator:
    nonconst_bidirectional_iterator
    // def: iterator until gobal end; mark bucket end diff from global end or have a seperate bucket iterator

   */

#include <utility>
#include <memory>
#include <functional>
#include <vector>
#include <list>
#include <istream>
#include <ostream>
#include <iostream>
#include <cmath>

#include "req.hh"
#include "utils.hh"

#define HTABLE_INIT_SIZE 50

int is_prime(long long);
int next_prime(int);

namespace iz {

    template <typename Key, typename Hash = std::hash<Key> >
    void print_hashed(const Key& key, unsigned size);

    template <typename Key, typename Val>
    using ht_item = std::pair<const Key, Val>;

    template <typename Key, typename Val>
    using bucket = std::list< ht_item<Key, Val> >;

    template <typename Key, typename Val>
    using buckets_vector = std::vector< bucket<Key, Val> >;

    template <typename Key, typename Val>
    const buckets_vector<Key, Val>&
    bucketcpy (buckets_vector<Key, Val>&, const buckets_vector<Key, Val>&);

    template <typename Key, typename Val, typename Hash>
    class htable;

    template <typename Key, typename Val, typename Hash>
    std::ostream& operator << (std::ostream&, htable<Key, Val, Hash>);

    template <typename Key, typename Val, typename Hash = std::hash<Key> >
    class htable
    {
    public:
        class iterator
        {
        private:
            typename bucket<Key, Val>::iterator current_item;
            typename buckets_vector<Key, Val>::iterator current_bucket;
            typename buckets_vector<Key, Val>::iterator buckets_end;
            bool init;

        public:
            iterator() {
                init = false;
            }

            iterator(
                    typename buckets_vector<Key, Val>::iterator begin,
                    typename buckets_vector<Key, Val>::iterator end
                    )
            {
                init = true;
                buckets_end = end;

                /* Skip empty buckets. */
                for (
                        current_bucket = begin;
                        current_bucket != end && (*current_bucket).empty();
                        ++current_bucket
                    )
                { ; }

                if (current_bucket != end) {
                    current_item = (*current_bucket).begin();
                }
            }

            /* HERE */
            iterator& operator ++ () {
                req(init);
                req(current_bucket != buckets_end); // delete_me

                if (current_bucket == buckets_end) {
                    return *this;
                }

                req(current_item != (*current_bucket).end());

                ++current_item;

                /* If current_bucketend() is reached, get next bucket. */
                if (current_item == (*current_bucket).end()) {
                    for (
                            ++current_bucket;
                            current_bucket != buckets_end && (*current_bucket).empty();
                            ++current_bucket
                        )
                    { ; }

                    std::cout << "HERE\n\n";
                    if (current_bucket != buckets_end) {
                        current_item = (*current_bucket).begin();
                    }
                }

                return *this;
            }

            iterator  operator ++ (int) {
                iterator ret_val = *this;
                ++(*this);
                return ret_val;
            }

            ht_item<Key, Val>& operator * () {
                req(init);
                req(current_bucket != buckets_end, "Trying to access null data.");
                req(current_item != (*current_bucket).end(), "[Debug]");

                return *current_item;
            }

            bool operator == (const iterator& other) const {
                req(init);
                if (current_bucket != other.current_bucket) {
                    return false;
                }

                if ( current_bucket == buckets_end ||
                     current_item == other.current_item )
                {
                    return true;
                }

                return false;
            }

            bool operator != (const iterator& other) const {
                return !(*this == other);
            }

        };

        iterator begin() {
            iterator itr(data.begin(), data.end());
            return itr;
        }
        iterator end() {
            iterator itr(data.end(), data.end());
            return itr;
        }


        htable(unsigned init_size = HTABLE_INIT_SIZE);

        Val& insert(const Key&, const Val&);
        void remove(const Key&);

        unsigned load() const;

        void resize(unsigned);
        void resize_up();
        void resize_down();

        static Hash hash;
        buckets_vector<Key, Val> data;

        void map(std::function<void(ht_item<Key, Val>&)>);
    private:

        unsigned size;
        unsigned base_size;
        unsigned count;
    };

    template <typename Key, typename Val, typename Hash>
    Hash htable<Key, Val, Hash>::hash;

    template <typename Key, typename Val, typename Hash>
    htable<Key, Val, Hash>::htable(unsigned init_size)
    {
        if (init_size < HTABLE_INIT_SIZE) {
            base_size = HTABLE_INIT_SIZE;
        }
        else {
            base_size = init_size;
        }

        size = next_prime(base_size);
        data.resize(size);

        count = 0;
    }

    /* */
    template <typename Key, typename Val, typename Hash>
    Val& htable<Key, Val, Hash>::insert(const Key& key, const Val& val)
    {
        print_green("Insert...\n");
        size_t key_hash;

        key_hash = hash(key) % size;
        print_hashed(key, size);

        for (auto& item : data[key_hash]) {
            if (item.first == key) {
                item.second = val;

                return item.second;
            }
        }

        data[key_hash].push_front(std::pair<const Key, Val>(key, val));
        ++count;


        std::cout << "Load: " << load() << '\n';
        if (load() > 70) {
            resize_up();
        }

        return data[key_hash].front().second;
    }

    /* */
    template <typename Key, typename Val, typename Hash>
    void htable<Key, Val, Hash>::remove(const Key& key)
    {
        print_green("remove...\n");
        size_t key_hash;

        key_hash = hash(key) % size;
        print_hashed(key, size);

        typename bucket<Key, Val>::iterator itr;
        for (
                itr = data[key_hash].begin();
                itr != data[key_hash].end();
                ++itr
            )
        {
            if ((*itr).first == key) {
                data[key_hash].erase(itr);
                --count;

                std::cout << "Load: " << load() << '\n';
                if (load() < 10) {
                    resize_down();
                }

                return;
            }
        }
        // Key was not found.
    }


    /* */
    template <typename Key, typename Val, typename Hash>
    void htable<Key, Val, Hash>::resize(unsigned new_base_size)
    {
        base_size = new_base_size;
        size = next_prime(base_size);

        buckets_vector<Key, Val> temp;

        bucketcpy(temp, data);
        data.clear();
        data.resize(size);

        for (auto& bucket : temp)
            for (auto& item : bucket)
                insert(item.first, item.second);

        temp.clear();
    }

    /* Double or half size. */
    template <typename Key, typename Val, typename Hash>
    void htable<Key, Val, Hash>::resize_up()
    {
        resize(base_size * 2);
    }
    template <typename Key, typename Val, typename Hash>
    void htable<Key, Val, Hash>::resize_down()
    {
        resize(base_size / 2);
    }

    /* Helper - Used only by resize. */
    template <typename Key, typename Val>
    const buckets_vector<Key, Val>&
    bucketcpy (buckets_vector<Key, Val>& dest, const buckets_vector<Key, Val>& src)
    {
        dest.clear();
        dest.resize(src.size());

        for (unsigned bucket_index = 0; bucket_index < src.size(); ++bucket_index) {
            for (const auto& item : src[bucket_index]) {
                dest[bucket_index].push_front(item);
            }
        }

        return dest;
    }

    /* Calc. load */
    template <typename Key, typename Val, typename Hash>
    unsigned htable<Key, Val, Hash>::load() const
    {
        return count * 100 / size;
    }

    /* */
    template <typename Key, typename Val, typename Hash>
    void htable<Key, Val, Hash>::map(std::function<void(ht_item<Key, Val>&)> action)
    {
        for (auto& bucket : data) {
            for (auto& item : bucket) {
                action(item);
            }
        }
    }


    /* Print */
    template <typename Key, typename Val, typename Hash>
    std::ostream& operator << (std::ostream& out, htable<Key, Val, Hash> ht)
    {
        ht.map([&](ht_item<Key, Val>& item) {
            out << "( " << item.first << ", " << item.second << " )\n";
        });

        return out;
    }


    template <typename Key, typename Hash>
    void print_hashed(const Key& key, unsigned size)
    {
        Hash hash;
        size_t key_hash;

        key_hash = hash(key)%size;
        std::cout << key << " => " << hash(key) << " mod " << size << " = " << key_hash << '\n';
    }
}


int is_prime(long long n)
{
    if (n < 2) {
        return -1;
    }
    if (n < 4) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }

    for (long long i = 3; i <= std::floor(std::sqrt(n)); i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int next_prime(int n)
{
    while (is_prime(n) != 1) {
        ++n;
    }
    return n;
}

#endif // !__hash_table_hh


/*
    template <typename Key, typename Val>
    const buckets_vector<Key, Val>&
    buckets_vector_map (buckets_vector<Key, Val>& data, std::function<void(ht_item<Key, Val>&)> action)
    {
        for (auto& bucket : data)
            for (auto& item : bucket)
                action(item);
    }
   */
