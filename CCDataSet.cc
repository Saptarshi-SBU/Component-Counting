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
#include "CCDataSet.hpp"
#include "CCImageReader.hpp"

CCDataSet::CCDataSet(const void *source, CCDataSourceType type) :
    CCDataObject(), source_(source), type_(type) {}

CCDataSet::~CCDataSet() {}

bool CCDataSet::Load(void) {
    bool done = false;
    switch (type_) {
    case CCDataSourceType::IMG: {
        // TBD : Fix attributes
        CCImageReader im(static_cast<const char*>(source_), CCImageSourceType::PNG);
        done = im.Load();
        if (done)
            dataItems_.push_back(im);
        break;
    }
    default:
        break;
    }
    return done;
}

bool CCDataSet::Destroy() {
    for (auto &i : dataItems_)
        i.Destroy();
    dataItems_.clear();
    return true;
}

int CCDataSet::getNumRecords() {
    return dataItems_.size();
}

CCDataSourceType CCDataSet::getSourceType(void) {
    return type_;
}
