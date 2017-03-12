//
// Created by Han Hu on 2/23/17.
//

#ifndef LIDARVISUALIZER_LASREADER_H
#define LIDARVISUALIZER_LASREADER_H

#include <stdlib.h>
#include <functional>
#include "LasDataset.h"
#include "LasPoint.h"

class LasReader;

class LasReaderIterator {
public:
    LasReader& m_lasReader;
    uint64_t m_pointIndex;
    LasReaderIterator(LasReader& lasReader, uint64_t pointIndex);

    // Copy assignment operator and constructor
    LasReaderIterator(const LasReaderIterator& other);

    // Move assignment operator and constructor
    LasReaderIterator(LasReaderIterator&& other);

    LasPoint & operator*() const;
    LasPoint * operator->() const;
    void operator++();
    void operator++(int);
    bool operator<(const LasReaderIterator& rhs) const;
};

class LasReaderConstIterator {
public:
    const LasReader& m_lasReader;
    uint64_t m_pointIndex;
    LasReaderConstIterator(const LasReader& lasReader, uint64_t pointIndex);
    LasReaderConstIterator(const LasReaderIterator& iterator);

    const LasPoint & operator*() const;
    const LasPoint * operator->() const;
    void operator++();
    void operator++(int);
    bool operator<(const LasReaderConstIterator& rhs) const;
};

class LasReader : public LasDataset{
public:
    void open(const char* fileName);
    void close();

    friend class LasReaderIterator;
    friend class LasReaderConstIterator;

    typedef LasReaderIterator iterator;
    typedef LasReaderConstIterator const_iterator;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    iterator rbegin();
    const_iterator rbegin() const;

    iterator rend();
    const_iterator rend() const;

    LasPoint & front();
    const LasPoint & front() const;

    LasPoint & back();
    const LasPoint & back() const;

    LasPoint & operator[](uint64_t n);
    const LasPoint & operator[](uint64_t n) const;

protected:
    mutable LasPoint m_lasPoint;
    mutable uint64_t m_pointIndex;

private:
    std::function<void(LasReader*)> m_fnReadPointDataRecord;

    void readPointDataRecord0();
    void readPointDataRecord1();
    void readPointDataRecord2();
    void readPointDataRecord3();
    void readPointDataRecord4();
    void readPointDataRecord5();
    void readPointDataRecord6();
    void readPointDataRecord7();
    void readPointDataRecord8();
    void readPointDataRecord9();
    void readPointDataRecord10();

    void readWavePacket();

    void readKthPoint(const uint64_t index) const;
};


#endif //LIDARVISUALIZER_LASDATASET_H
