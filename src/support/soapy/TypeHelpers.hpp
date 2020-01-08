// Copyright (c) 2014-2018 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#pragma once
#include "Config.hpp"
#include "Types.hpp"
#include "Types.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>

/*******************************************************************
 * Helpful converters
 ******************************************************************/
static inline char *toCString(const std::string &s)
{
    auto out = (char *)calloc(s.size()+1, sizeof(char));
    if (out == nullptr) throw std::bad_alloc();
    std::copy(s.begin(), s.end(), out);
    return out;
}

static inline char **toStrArray(const std::vector<std::string> &strs, size_t *length)
{
    auto out = (char **)calloc(strs.size(), sizeof(char *));
    if (out == nullptr) throw std::bad_alloc();
    for (size_t i = 0; i < strs.size(); i++)
    {
        try
        {
            out[i] = toCString(strs[i]);
        }
        catch (const std::bad_alloc &)
        {
            SoapySDRStrings_clear(&out, i);
            throw;
        }
    }
    *length = strs.size();
    return out;
}

static inline SoapySDRRange toRange(const SoapySDR::Range &range)
{
    SoapySDRRange out;
    out.minimum = range.minimum();
    out.maximum = range.maximum();
    out.step = range.step();
    return out;
}

static inline SoapySDRRange *toRangeList(const SoapySDR::RangeList &ranges, size_t *length)
{
    auto out = (SoapySDRRange *)calloc(ranges.size(), sizeof(SoapySDRRange));
    if (out == nullptr) throw std::bad_alloc();
    for (size_t i = 0; i < ranges.size(); i++) out[i] = toRange(ranges[i]);
    *length = ranges.size();
    return out;
}

static inline double *toNumericList(const std::vector<double> &values, size_t *length)
{
    auto out = (double *)calloc(values.size(), sizeof(double));
    if (out == nullptr) throw std::bad_alloc();
    std::copy(values.begin(), values.end(), out);
    *length = values.size();
    return out;
}

static inline SoapySDR::Kwargs toKwargs(const SoapySDRKwargs *args)
{
    SoapySDR::Kwargs out;
    if (args == NULL) return out;
    for (size_t i = 0; i < args->size; i++)
    {
        out[args->keys[i]] = args->vals[i];
    }
    return out;
}

static inline SoapySDRKwargs toKwargs(const SoapySDR::Kwargs &args)
{
    SoapySDRKwargs out;
    std::memset(&out, 0, sizeof(out));
    for (const auto &it : args)
    {
        if (SoapySDRKwargs_set(&out, it.first.c_str(), it.second.c_str()) != 0)
        {
            throw std::bad_alloc();
        }
    }
    return out;
}

static inline SoapySDRKwargs *toKwargsList(const SoapySDR::KwargsList &args, size_t *length)
{
    auto outArgs = (SoapySDRKwargs *)calloc(args.size(), sizeof(SoapySDRKwargs));
    if (outArgs == nullptr) throw std::bad_alloc();
    for (size_t i = 0; i < args.size(); i++) outArgs[i] = toKwargs(args[i]);
    *length = args.size();
    return outArgs;
}

static inline SoapySDR::KwargsList toKwargsList(const SoapySDRKwargs *args, const size_t length)
{
    SoapySDR::KwargsList outArgs(length);
    for (size_t i = 0; i < length; i++) outArgs[i] = toKwargs(args+i);
    return outArgs;
}

static inline SoapySDRArgInfo toArgInfo(const SoapySDR::ArgInfo &info)
{
    SoapySDRArgInfo out;
    std::memset(&out, 0, sizeof(out));
    try
    {
        out.key = toCString(info.key);
        out.value = toCString(info.value);
        out.name = toCString(info.name);
        out.description = toCString(info.description);
        out.units = toCString(info.units);
        out.type = SoapySDRArgInfoType(info.type);
        out.range = toRange(info.range);
        out.options = toStrArray(info.options, &out.numOptions);
        out.optionNames = toStrArray(info.optionNames, &out.numOptions);
    }
    catch (const std::bad_alloc &)
    {
        SoapySDRArgInfo_clear(&out);
        throw;
    }

    return out;
}

static inline SoapySDRArgInfo *toArgInfoList(const SoapySDR::ArgInfoList &infos, size_t *length)
{
    auto out = (SoapySDRArgInfo *)calloc(infos.size(), sizeof(SoapySDRArgInfo));
    if (out == nullptr) throw std::bad_alloc();
    for (size_t i = 0; i < infos.size(); i++)
    {
        out[i] = toArgInfo(infos[i]);
    }
    *length = infos.size();
    return out;
}

static inline std::vector<unsigned> toNumericVector(const unsigned *values, size_t length)
{
    std::vector<unsigned> out (length, 0);
    std::copy(values, values+length, out.data());
    return out;
}

static inline unsigned *toNumericList(const std::vector<unsigned> &values, size_t *length)
{
    auto out = (unsigned *)calloc(values.size(), sizeof(unsigned));
    if (out == nullptr) throw std::bad_alloc();
    std::copy(values.begin(), values.end(), out);
    *length = values.size();
    return out;
}

