#ifndef __hash_table_hh
#define __hash_table_hh

/*
   hash< Key, Val, Hash >
container:
    std::vector< std::list<Val> >;

[WAIT] methods:
    delete_objs_also(Key)
    delete_arrays_also(Key)
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

	/* Aliases */
    template <typename Key, typename Val>
    using ht_item = std::pair<const Key, Val>;

    template <typename Key, typename Val>
    using bucket = std::list< ht_item<Key, Val> >;

    template <typename Key, typename Val>
    using buckets_vector = std::vector< bucket<Key, Val> >;

	/* bucketcpy(dest, src) */
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
	private:
		unsigned size;
		unsigned base_size;
		unsigned __collision_count;
		unsigned __count;

    public:
		static Hash hash;
		buckets_vector<Key, Val> data;

        htable(unsigned init_size = HTABLE_INIT_SIZE);

        Val& insert(const Key&, const Val&, bool rewrite = true);
		Val& operator [] (const Key&);

        void remove(const Key&);

        void resize(unsigned);
        void resize_up();
        void resize_down();

		static ht_item<Key, Val>* static_search(buckets_vector<Key, Val>& data, const Key&);
        void map(std::function<void(ht_item<Key, Val>&)>);

        unsigned load() const;
		unsigned count() const;

		/* Collision stats */
		void reset_collision_count();
		unsigned collision_count();

		/* Iterators */
		class bucket_iterator
		{
		private:
			typename bucket<Key, Val>::iterator current_item;
			typename bucket<Key, Val>::iterator bucket_end;
			bool init;

		public:
			bucket_iterator();

			bucket_iterator(
				typename buckets_vector<Key, Val>::iterator begin,
				typename buckets_vector<Key, Val>::iterator end,
				const Key& key
			);

			bool reached_end() const;

			bucket_iterator& operator ++ ();
			bucket_iterator operator ++ (int);

			bool operator == (const bucket_iterator& other) const;
			bool operator != (const bucket_iterator& other) const;

			ht_item<Key, Val>& operator * ();

			bucket_iterator& find(  typename buckets_vector<Key, Val>::iterator begin,
									typename buckets_vector<Key, Val>::iterator end,
									const Key& key );
		};

		class iterator
		{
			friend class htable<Key, Val, Hash>::bucket_iterator;
		private:
			typename bucket<Key, Val>::iterator current_item;
			typename buckets_vector<Key, Val>::iterator current_bucket;
			typename buckets_vector<Key, Val>::iterator buckets_end;
			bool init;

		public:
			iterator();

			iterator(typename buckets_vector<Key, Val>::iterator begin,
				typename buckets_vector<Key, Val>::iterator end);

			iterator& operator ++ ();
			iterator  operator ++ (int);

			bool operator == (const iterator& other) const;
			bool operator != (const iterator& other) const;

			ht_item<Key, Val>& operator * ();
			iterator& find(const Key& key);

			bool reached_end() const;
		};

		bucket_iterator find_bucket(const Key& key) {
			bucket_iterator itr;
			itr.find(data.begin(), data.end(), key);
			return itr;
		}

		iterator find(const Key& key) {
			iterator itr(data.begin(), data.end());
			itr.find(key);
			return itr;
		}

		iterator begin() {
			iterator itr(data.begin(), data.end());
			return itr;
		}
		iterator end() {
			iterator itr(data.end(), data.end());
			return itr;
		}
		/* end iterator */
    };

	template <typename Key, typename Val, typename Hash>
	unsigned htable<Key, Val, Hash>::count() const
	{
		return __count;
	}

	template <typename Key, typename Val, typename Hash>
	unsigned htable<Key, Val, Hash>::collision_count()
	{
		return __collision_count;
	}

	template <typename Key, typename Val, typename Hash>
	void htable<Key, Val, Hash>::reset_collision_count()
	{
		__collision_count = 0;
	}

    template <typename Key, typename Val, typename Hash>
    Hash htable<Key, Val, Hash>::hash;

    template <typename Key, typename Val, typename Hash>
    htable<Key, Val, Hash>::htable(unsigned init_size)
    {
		__collision_count = 0;

        if (init_size < HTABLE_INIT_SIZE) {
            base_size = HTABLE_INIT_SIZE;
        }
        else {
            base_size = init_size;
        }

        size = next_prime(base_size);
        data.resize(size);

        __count = 0;
    }

    /* */
    template <typename Key, typename Val, typename Hash>
    Val& htable<Key, Val, Hash>::insert(const Key& key, const Val& val, bool rewrite)
    {
        //print_green("Insert...\n");
        size_t key_hash;

        key_hash = hash(key) % size;
        print_hashed(key, size);

        for (auto& item : data[key_hash]) {
            if (item.first == key) {
				if (rewrite) {
					item.second = val;
				}

                return item.second;
            }
        }

		/* Collision stat */
		if (!data[key_hash].empty()) {
			__collision_count += 1;
		}

        data[key_hash].push_front(std::pair<const Key, Val>(key, val));
        ++__count;


        //std::cout << "Load: " << load() << '\n';
        if (load() > 70) {
            resize_up();
			return (*static_search(data, key)).second; // hash of key might've changed (%size) :D
        }

        return data[key_hash].front().second;
    }

	template <typename Key, typename Val, typename Hash>
	Val& htable<Key, Val, Hash>::operator [] (const Key& key)
	{
		Val null_val{};
		return insert(key, null_val, false);
	}

    /* */
    template <typename Key, typename Val, typename Hash>
    void htable<Key, Val, Hash>::remove(const Key& key)
    {
        //print_green("remove...\n");
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
                --__count;

                //std::cout << "Load: " << load() << '\n';
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

	/* */
	template <typename Key, typename Val, typename Hash>
	ht_item<Key, Val>* htable<Key, Val, Hash>::static_search(buckets_vector<Key, Val>& data, const Key& key)
	{
		//print_green("Search...\n");
		size_t key_hash;

		key_hash = hash(key) % data.size();
		print_hashed(key, data.size());

		for (auto& item : data[key_hash]) {
			if (item.first == key) {
				return &item;	// &ref == &referred
			}
		}
		return nullptr;
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
        return __count * 100 / size;
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
		for (const auto& item : ht) {
			out << "( " << item.first << ", " << item.second << " )\n";
		}

        return out;
    }


	/* @Begin Iterator */
	template <typename Key, typename Val, typename Hash>
	htable<Key, Val, Hash>::iterator
		::iterator()
	{
		init = false;
	}

	template <typename Key, typename Val, typename Hash>
	htable<Key, Val, Hash>::iterator
		::iterator(typename buckets_vector<Key, Val>::iterator begin,
			typename buckets_vector<Key, Val>::iterator end)
	{
		init = true;
		buckets_end = end;

		/* Skip empty buckets. */
		for (
			current_bucket = begin;
			!reached_end() && (*current_bucket).empty();
			++current_bucket
			)
		{ ; }

		if (!reached_end()) {
			current_item = (*current_bucket).begin();
		}
	}

	template <typename Key, typename Val, typename Hash>
	typename htable<Key, Val, Hash>::iterator&
		htable<Key, Val, Hash>::iterator
		::operator ++ ()
	{
		req(init);

		if (reached_end()) {
			return *this;
		}

		req(current_item != (*current_bucket).end(), "[Debug]");

		/* Next */
		++current_item;

		/* If (*current_bucket).end() is reached, get next bucket. */
		if (current_item == (*current_bucket).end()) {
			for (
				++current_bucket;
				!reached_end() && (*current_bucket).empty();
				++current_bucket
				)
			{ ; }

			if (!reached_end()) {
				current_item = (*current_bucket).begin();
			}
		}

		return *this;
	}

	template <typename Key, typename Val, typename Hash>
	typename htable<Key, Val, Hash>::iterator
		htable<Key, Val, Hash>::iterator
		::operator ++ (int)
	{
		iterator ret_val = *this;
		++(*this);
		return ret_val;
	}

	template <typename Key, typename Val, typename Hash>
	ht_item<Key, Val>&
		htable<Key, Val, Hash>::iterator
		::operator * ()
	{
		req(init);
		req(!reached_end(), "Trying to access null data.");
		req(current_item != (*current_bucket).end(), "[Debug]");

		return *current_item;
	}

	template <typename Key, typename Val, typename Hash>
	bool
		htable<Key, Val, Hash>::iterator
		::operator == (const iterator& other) const
	{
		req(init);

		if (current_bucket != other.current_bucket) {
			return false;
		}

		if (reached_end() || current_item == other.current_item) {
			return true;
		}

		return false;
	}

	template <typename Key, typename Val, typename Hash>
	bool
		htable<Key, Val, Hash>::iterator
		::operator != (const iterator& other) const
	{
		return !(*this == other);
	}

	template <typename Key, typename Val, typename Hash>
	typename htable<Key, Val, Hash>::iterator&
		htable<Key, Val, Hash>::iterator
		::find(const Key& key)
	{
		req(init);

		/* Skip until key. */
		while (!reached_end()) {
			if (key == (*current_item).first) {
				break;
			}
			++(*this);
		}

		return *this;
	}

	template <typename Key, typename Val, typename Hash>
	bool
		htable<Key, Val, Hash>::iterator
		::reached_end() const
	{
		req(init);
		return current_bucket == buckets_end;
	}
	/* @End iterator */


	/* @Begin Bucket Iterator */
	template <typename Key, typename Val, typename Hash>
	htable<Key, Val, Hash>::bucket_iterator
		::bucket_iterator()
	{
		init = false;
	}

	template <typename Key, typename Val, typename Hash>
	htable<Key, Val, Hash>::bucket_iterator
		::bucket_iterator(
			typename buckets_vector<Key, Val>::iterator begin,
			typename buckets_vector<Key, Val>::iterator end,
			const Key& key
		)
	{
		find(begin, end, key);
	}


	template <typename Key, typename Val, typename Hash>
	typename htable<Key, Val, Hash>::bucket_iterator&
		htable<Key, Val, Hash>::bucket_iterator
		::operator ++ ()
	{
		req(init);

		if (!reached_end()) {
			++current_item;
		}
		return *this;
	}

	template <typename Key, typename Val, typename Hash>
	typename htable<Key, Val, Hash>::bucket_iterator
		htable<Key, Val, Hash>::bucket_iterator
		::operator ++ (int)
	{
		bucket_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}

	template <typename Key, typename Val, typename Hash>
	ht_item<Key, Val>&
		htable<Key, Val, Hash>::bucket_iterator
		::operator * ()
	{
		req(init);
		req(!reached_end(), "Trying to access null data.");
		return *current_item;
	}

	template <typename Key, typename Val, typename Hash>
	bool
		htable<Key, Val, Hash>::bucket_iterator
		::operator == (const bucket_iterator& other) const
	{
		req(init);
		return current_item != other.current_item;
	}

	template <typename Key, typename Val, typename Hash>
	bool
		htable<Key, Val, Hash>::bucket_iterator
		::operator != (const bucket_iterator& other) const
	{
		return !(*this == other);
	}

	template <typename Key, typename Val, typename Hash>
	typename htable<Key, Val, Hash>::bucket_iterator&
		htable<Key, Val, Hash>::bucket_iterator
		::find(
			typename buckets_vector<Key, Val>::iterator begin,
			typename buckets_vector<Key, Val>::iterator end,
			const Key& key
		)
	{
		htable<Key, Val, Hash>::iterator itr(begin, end);
		itr.find(key);

		if (!itr.reached_end()) {
			current_item = (*(itr.current_bucket)).begin();
			bucket_end = (*(itr.current_bucket)).end();
			init = true;
		}
		else {
			init = false;
		}

		return *this;
	}

	template <typename Key, typename Val, typename Hash>
	bool
		htable<Key, Val, Hash>::bucket_iterator
		::reached_end() const
	{
		req(init);
		return current_item == bucket_end;
	}
	/* @End Bucket bucket_iterator */


    template <typename Key, typename Hash>
    void print_hashed(const Key& key, unsigned size)
    {
        Hash hash;
        size_t key_hash;

        key_hash = hash(key)%size;
        //std::cout << key << " => " << hash(key) << " mod " << size << " = " << key_hash << '\n';
    }
}

/* Helpers */
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