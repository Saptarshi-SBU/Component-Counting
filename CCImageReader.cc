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

#include "CCImageReader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "CCLogger.hpp"

//If class members are neither mentioned in a constructor’s member initializer
//list nor have a brace-or-equal-initializer, then they get default-initialized.
//That means, that for class types the default constructor is called, but for
//any other types like enums or built in types like int, double, pointers, no
//initialization happens at all.
//
//This applies for each element of array, and, as a corollary, it applies for
//plain old data classes as well, as their default constructor in turn
//default-initializes all of their members. No initialization means your
//member variables possibly contain garbage values.

CCImageReader::CCImageReader():
    CCDataObject(),
    type_(CCImageSourceType::UNSUPPORTED),
    desiredChannels_(CCColorChannels::DEFAULT) {}

CCImageReader::CCImageReader(const char *filename, CCImageSourceType type) :
    CCDataObject(),
    filename_(std::string(filename)),
    type_(type),
    desiredChannels_(CCColorChannels::DEFAULT) {}

CCImageReader::CCImageReader(const char *filename, CCImageSourceType type, CCColorChannels channels) :
    CCDataObject(),
    filename_(std::string(filename)),
    type_(type),
    desiredChannels_(channels) {}

CCImageReader::CCImageReader(const CCImageReader &srcImg) {
    width_  = srcImg.width_;
    height_ = srcImg.height_;
    type_ = srcImg.type_;
    numChannels_ = srcImg.numChannels_;
    desiredChannels_ = srcImg.desiredChannels_;
    data_ = (unsigned char *) stbi__malloc(sizeof(unsigned char) * width_ * height_ * numChannels_);
    memcpy(data_, srcImg.data_, sizeof(unsigned char) * width_ * height_ * numChannels_);
    std::cout << "numChannels :" << numChannels_ << std::endl;
}

CCImageReader::~CCImageReader() {}

void CCImageReader::setFilename(const char *filename) {
    filename_ = std::string(filename);
}

const char * CCImageReader::getFilename(void) {
    return filename_.c_str();
}

int CCImageReader::getWidth() {
    return width_;
}

void CCImageReader::setWidth(int width) {
    width_ = width;
}

int CCImageReader::getHeight() {
    return height_;
}

void CCImageReader::setHeight(int height) {
    height_ = height;
}

int CCImageReader::getNumChannels() {
    return numChannels_;
}

void CCImageReader::setNumChannels(int numChannels) {
    numChannels_ = numChannels;
}

CCColorChannels CCImageReader::getColorChannels() {
    return desiredChannels_;
}

void CCImageReader::setColorChannels(CCColorChannels colorChannel) {
    desiredChannels_ = colorChannel;
}

unsigned char* CCImageReader::getDataBlob() {
    return data_;
}

void CCImageReader::setDataBlob(unsigned char *data) {
    data_ = data;
}

int CCImageReader::getSize(void) {
    return width_ * height_;
}

CCImageSourceType CCImageReader::getFormat(void) {
    return type_;
}

void CCImageReader::setFormat(CCImageSourceType type) {
    type_ = type;
}

bool CCImageReader::Load() {
    if (type_ >= CCImageSourceType::UNSUPPORTED)
        goto error;

    if (data_ != nullptr)
        goto skip;

    switch (desiredChannels_) {
    case CCColorChannels::DEFAULT:
        data_ = stbi_load(filename_.c_str(), &width_, &height_, &numChannels_, 0);
        break;
    case CCColorChannels::GRAY:
        data_ = stbi_load(filename_.c_str(), &width_, &height_, &numChannels_, STBI_grey);
        break;
    case CCColorChannels::GRAY2:
        data_ = stbi_load(filename_.c_str(), &width_, &height_, &numChannels_, STBI_grey_alpha);
        break;
    case CCColorChannels::RGB:
        data_ = stbi_load(filename_.c_str(), &width_, &height_, &numChannels_, STBI_rgb);
        break;
    case CCColorChannels::RGBA:
        data_ = stbi_load(filename_.c_str(), &width_, &height_, &numChannels_, STBI_rgb_alpha);
        break;
    default:
        assert(0);
        break;
    }

skip:
    if (data_ != nullptr)
        return true;

error:
    return false;
}

std::string CCImageReader::generateUUIDName(void) {
    CCUUid ccuid = UUidInfo();
    std::string str = ccuid.getString();

    if (type_ == CCImageSourceType::PNG)
        str.append(".png");
    else if (type_ == CCImageSourceType::BMP) 
        str.append(".bmp");
    else if (type_ == CCImageSourceType::JPG) 
        str.append(".jpg");
    return str;
}

bool CCImageReader::Save() {
    int ret;
    std::string name;

    if ((data_ == nullptr) || (width_ == 0) || (height_ == 0))
        goto error;

    if (filename_.empty()) {
        name = generateUUIDName();
    } else
        name = std::string(filename_);

    switch (type_) {
    case CCImageSourceType::PNG:
        ret = stbi_write_png(name.c_str(), width_, height_, numChannels_, data_, width_ * numChannels_);
        break;
    case CCImageSourceType::BMP:
        ret = stbi_write_bmp(name.c_str(), width_, height_, numChannels_, data_);
        break;
    case CCImageSourceType::JPG:
        ret = stbi_write_jpg(name.c_str(), width_, height_, numChannels_, data_, CCJPEG_LOSS);
    default:
        ret = 0;
        break;
    }
    return ret > 0;

error:
    return false;
}

bool CCImageReader::Destroy() {
    if (data_) {
        stbi_image_free(data_);
        data_= nullptr;
    }
    return true;
}

CCImageReader CCImageReader::ConvertRGB2GRAY(bool &ok) {
    CCImageReader newImg;
    CCColorChannels cchannels;
    unsigned char *pSrc, *pData;

    newImg.setWidth(getWidth());
    newImg.setHeight(getHeight());
    newImg.setFormat(getFormat());

    cchannels = getColorChannels();
    if (cchannels == CCColorChannels::RGBA) {
        newImg.setNumChannels(2);
        newImg.setColorChannels(CCColorChannels::GRAY2);
    } else if (cchannels == CCColorChannels::RGB) {
        newImg.setNumChannels(1);
        newImg.setColorChannels(CCColorChannels::GRAY);
    } else {
        CC_ERR("image is already gray-scale");
        goto error;
    }

    pData = (unsigned char *) stbi__malloc(sizeof(unsigned char) * newImg.getWidth() *
        newImg.getHeight() * newImg.getNumChannels());
    if (!pData)
        goto error;

    newImg.setDataBlob(pData);
    pSrc = getDataBlob();

    for (int count = 0; count < getSize(); pSrc += getNumChannels(),
            pData += newImg.getNumChannels(), count++) {
        *pData = (*pSrc + *(pSrc + 1) + *(pSrc + 2)) / 3.0;
        if (cchannels == CCColorChannels::RGBA)
            *(pData + 1) = *(pSrc + 3);
    }

    ok = true;
    return newImg;

error:
    ok = false;
    return newImg;
}

