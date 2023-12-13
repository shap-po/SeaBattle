#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <vector>
#include <set>

void setSeed();
void setSeed(int seed);

int randInt(int min, int max);
int randInt(int max);

template <typename T>
T randElement(const std::vector<T>& vec)
{
    return vec[randInt(vec.size())];
}

template <typename T>
T randElement(const std::set<T>& set)
{
    return *std::next(set.begin(), randInt(set.size()));
}

#endif // RANDOM_HPP_
