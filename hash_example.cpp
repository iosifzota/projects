namespace std {
    template<typename Key>
    class hash< pair<Key, Key> > {
    public:
        size_t operator() (const std::pair<Key, Key>& edge) const
        {
            size_t first = hash(edge.first);
            size_t second = hash(edge.second);

            return (first < second) ? first : second;
        }
    };
}
