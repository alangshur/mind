#include <iostream>
#include "util/iqr-scorer.hpp"
using namespace std;

void IQRScorer::update_even_quartile_it(quartile_t& quartile, 
    bool less_than_quartile) {
    if (less_than_quartile) {
        quartile = {
            double(*(quartile.it_f) + *prev(quartile.it_f)) / 2.0,
            true,
            prev(quartile.it_f),
            quartile.it_f
        };
    }
    else {
        quartile = {
            double(*(quartile.it_f) + *next(quartile.it_f)) / 2.0,
            true,
            quartile.it_f,
            next(quartile.it_f)
        };
    }
}

void IQRScorer::update_odd_quartile_it(quartile_t& quartile, 
    bool less_than_quartile) {
    if (less_than_quartile) {
        quartile = {
            double(*(this->q2.it_f)),
            false,
            this->q2.it_f,
            this->second_quartile_set.end()
        };
    }
    else {
        quartile = {
            double(*next(this->q2.it_f)),
            false,
            next(this->q2.it_f),
            this->second_quartile_set.end()
        };
    }
}

void IQRScorer::update_quartile(uint32_t sample, quartile_t& quartile,
    multiset<uint32_t>& quartile_set) {

    // update specified set quartile 
    if (quartile_set.size() % 2) {
        if (double(sample) >= *(quartile.it_f)) 
            this->update_odd_quartile_it(quartile, false);
        else this->update_odd_quartile_it(quartile, true);
    }
    else {
        if (double(sample) >= *(quartile.it_f)) 
            this->update_even_quartile_it(quartile, false);
        else this->update_even_quartile_it(quartile, true);
    }
}

void IQRScorer::add_sample(uint32_t sample) {
    this->sample_sum += sample;

    // initiate scorer
    if (!this->second_quartile_set.size()) {
        auto q1_it = this->first_quartile_set.insert(sample);
        auto q2_it = this->second_quartile_set.insert(sample);
        auto q3_it = this->third_quartile_set.insert(sample);
        this->q1 = { double(sample), false, q1_it, q1_it };
        this->q2 = { double(sample), false, q2_it, q2_it };
        this->q3 = { double(sample), false, q3_it, q3_it };
    }

    // add to scorer 
    else {

        // re-insert samples
        if (this->q2.is_composite_quartile) {
            if (sample >= *(this->q2.it_s)) {
                this->first_quartile_set.insert(*(this->q2.it_f));
                this->third_quartile_set.insert(sample);
            }
            else if (sample < *(this->q2.it_f)) {
                this->first_quartile_set.insert(sample);
                this->third_quartile_set.insert(*(this->q2.it_s));
            }
            else {
                this->first_quartile_set.insert(*(this->q2.it_f));
                this->third_quartile_set.insert(*(this->q2.it_s));
            }
        }
        else {
            if (sample >= *(this->q2.it_f)) {
                this->first_quartile_set.insert(*(this->q2.it_f));
                this->third_quartile_set.insert(sample);
            }
            else {
                this->first_quartile_set.insert(sample);
                this->third_quartile_set.insert(*(this->q2.it_f));
            }
        }

        // update quartiles
        this->second_quartile_set.insert(sample);
        this->update_quartile(sample, this->q1, this->first_quartile_set);
        this->update_quartile(sample, this->q2, this->second_quartile_set);
        this->update_quartile(sample, this->q3, this->third_quartile_set);
    }
}

void IQRScorer::remove_sample(uint32_t sample) {
    auto it = this->second_quartile_set.find(sample);
    if (it == this->second_quartile_set.end()) 
        throw runtime_error("Sample not found in IQR scorer");
    this->sample_sum -= sample;
}

double IQRScorer::get_iqr() {
    return this->get_q3() - this->get_q1();
}

double IQRScorer::get_q1() {
    return (this->q1).value;
}

double IQRScorer::get_q2() {
    return (this->q2).value;
}

double IQRScorer::get_q3() {
    return (this->q3).value;
}

double IQRScorer::get_mean() {
    return double(this->sample_sum)
        / double(this->second_quartile_set.size());
}

bool IQRScorer::is_outlier() {
    return false;
}

void IQRScorer::print_quartile_set() {

    // print first quartile
    auto curr_it = this->first_quartile_set.begin();
    cout << *curr_it;
    while (++curr_it != this->first_quartile_set.end())
        cout << " - " << *curr_it;
    cout << endl << flush;

    // print second quartile
    curr_it = this->second_quartile_set.begin();
    cout << *curr_it;
    while (++curr_it != this->second_quartile_set.end())
        cout << " - " << *curr_it;
    cout << endl << flush;

    // print third quartile
    curr_it = this->third_quartile_set.begin();
    cout << *curr_it;
    while (++curr_it != this->third_quartile_set.end())
        cout << " - " << *curr_it;
    cout << endl << flush;
}   