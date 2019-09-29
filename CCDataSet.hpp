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
 *  DataSet : An immutable container of data. The container can be fed from
 *  data prepared offline (e.g. image files, *.dat files) or data provided
 *  at run-time (say data from camera frame).
 *
 */
#ifndef _CCDATASET_HPP_
#define _CCDATASET_HPP_

#include <list>
#include <memory>

#include "CCDataObject.hpp"
#include "CCDataSource.hpp"

class CCDataSet : public CCDataObject {

    public:

    CCDataSet(const void *, CCDataSourceType);

    ~CCDataSet();

    bool Load(void);

    bool Destroy(void);

    int getNumRecords(void);

    CCDataSourceType getSourceType(void);

    private:

    // source
    const void * source_;

    // number of records
    int numRecords_;

    // source type
    CCDataSourceType type_;

    // list of objects tracked by dataset
    std::list<CCDataObject> dataItems_;
};

#endif
