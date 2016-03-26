#ifndef MYSORT_H
#define MYSORT_H

namespace my {

    template <typename T>
    T pivot_value(T* array, int l, int r) {
        int index = (l + r) / 2;
        if (r - l > 10) {
            index = l + (std::rand()) % (r - l + 1);
        }
        return array[index];
    }

    template <typename T>
    void sort(T* array, const int left, const int right) {
        int i = left, j = right;
        int l = left, r = right;
        while (i < r) {
            T x = pivot_value(array, l, r);
            while (i < j) {
                while (array[i] < x) {i++;}
                while (x < array[j]) {j--;}
                if (i <= j) {
                    std::swap(array[i], array[j]);
                    i++;
                    j--;
                }
            }
            if (l < j) {
                sort(array, l, j);
            }
            l = i;
            j = r;
        }
    }
}
#endif // MYSORT_H
