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
#define _POSIX_SOURCE

#include <dirent.h>

#include <list>
#include <iostream>
#include <cassert>

#include "CCDataSet.hpp"
#include "CCImageReader.hpp"

CCDataSet::CCDataSet(const void *source, CCDataSourceType type) :
    CCDataObject(), source_(source), type_(type) {}

CCDataSet::~CCDataSet() {}

bool CCDataSet::LoadFile(void) {
    bool done = false;
    switch (type_) {
    case CCDataSourceType::IMG: {
        // TBD : Fix attributes
        CCImageReader *im = new CCImageReader(static_cast<const char*>(source_), CCImageSourceType::PNG);
        done = im->Load();
        if (done)
            dataItems_.push_back(im);
        break;
    }
    default:
        break;
    }
    return done;
}

bool CCDataSet::LoadDirectory(void) {
    DIR *dir;
    bool done = false;
    struct dirent *entry;
    std::list<struct dirent *> direntList;

    dir = opendir(static_cast<const char *>(source_));
    if (dir == nullptr)
        goto error;

    while ((entry = readdir(dir)) != nullptr)
        direntList.push_back(entry);

    switch (type_) {
    case CCDataSourceType::IMG: {
        for (auto &i : direntList) {
            if ((strcmp(i->d_name, ".") != 0) && (strcmp(i->d_name, "..") != 0)) {
                std::string path(static_cast<const char*>(source_));
                path.append(i->d_name);
                CCImageReader *imp = new CCImageReader(path.c_str(), CCImageSourceType::PNG);
                done = imp->Load();
                if (!done) {
                    std::cerr << path << std::endl;
                    assert(0);
                }
                // prevent object slicing
                dataItems_.push_back(dynamic_cast<CCDataObject *>(imp));
                std::cerr << "new image :" << imp->getFilename() << std::endl;
            }
        }

        for (auto &i : dataItems_) {
            CCImageReader *im = dynamic_cast<CCImageReader*>(i);
            std::cout << "processing image :" << im << ":" << im->getFilename() << std::endl;
        }
        break;
    }
    default:
        break;
    }

    closedir(dir);
error:
    return done;
}

bool CCDataSet::Destroy() {
    for (auto &item : dataItems_) {
        item->Destroy();
        delete item;
    }
    dataItems_.clear();
    return true;
}

int CCDataSet::getNumRecords() {
    return dataItems_.size();
}

CCDataSourceType CCDataSet::getSourceType(void) {
    return type_;
}
