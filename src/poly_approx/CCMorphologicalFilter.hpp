/*
 * MIT License
 *
 * Copyright (c) 2019 Saptarshi Sen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 *  Morph Filter
 *
 */

#ifndef _CCMORPHFILTER_HPP_
#define _CCMORPHFILTER_HPP_

#include "CCImageReader.hpp"

class CCMorphologicalFilter {

    public:

    CCMorphologicalFilter() : m_(3), n_(3), thresh_(255) {
    }

    CCMorphologicalFilter(int m, int n, int thresh) :
        m_(m), n_(n), thresh_(thresh) {
    }

    CCMorphologicalFilter(const CCMorphologicalFilter &er) {
        m_ = er.m_;
        thresh_ = er.thresh_;
    }

    CCMorphologicalFilter& operator=(const CCMorphologicalFilter &er) {
        return *this;
    }

    virtual ~CCMorphologicalFilter() {
    }

    virtual void Run(CCImageReader &img) {
        assert(0);
    }

    protected:

    int m_; // row

    int n_; // col

    int thresh_; // pixel threshold
};
#endif
