#ifndef IQR_SCORER_H
#define IQR_SCORER_H

#include <cstdint>
#include <utility>
#include <set>

typedef struct {
    double value;
    bool is_composite_quartile;
    std::multiset<uint32_t>::iterator it_f;
    std::multiset<uint32_t>::iterator it_s;
} quartile_t;

class IQRScorer {
    public:
        void add_sample(uint32_t sample);
        void remove_sample(uint32_t sample);
        double get_iqr();
        double get_q1();
        double get_q2();
        double get_q3();
        double get_mean();
        bool is_outlier();
        void print_quartile_set();

    private:
        void update_even_quartile_it(quartile_t& quartile, bool less_than_quartile);
        void update_odd_quartile_it(quartile_t& quartile, bool less_than_quartile);
        void update_quartile(uint32_t sample, quartile_t& quartile,
            std::multiset<uint32_t>& quartile_set);

        quartile_t q1, q2, q3;
        std::multiset<uint32_t> first_quartile_set;
        std::multiset<uint32_t> second_quartile_set;
        std::multiset<uint32_t> third_quartile_set;
        uint32_t sample_sum;
};

#endif