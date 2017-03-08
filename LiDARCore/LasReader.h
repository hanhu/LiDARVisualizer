//
// Created by Han Hu on 2/23/17.
//

#ifndef LIDARVISUALIZER_LASREADER_H
#define LIDARVISUALIZER_LASREADER_H

#include <stdlib.h>
#include <functional>
#include "LasDataset.h"

class LasReader;

class LasReaderIterator {
public:
    LasReader& m_lasReader;
    uint64_t m_pointIndex;
    LasReaderIterator(LasReader& lasReader, uint64_t pointIndex);

    POINT_DATA_RECORD& operator *() const;
    POINT_DATA_RECORD* operator->() const;
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

    const POINT_DATA_RECORD& operator*() const;
    const POINT_DATA_RECORD* operator->() const;
    void operator++();
    void operator++(int);
    bool operator<(const LasReaderConstIterator& rhs) const;
};

class LasReader : public LasDataset{
public:
    LasReader();
    virtual ~LasReader();

    void open(const char* fileName);

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

    POINT_DATA_RECORD& front();
    const POINT_DATA_RECORD& front() const;

    POINT_DATA_RECORD& back();
    const POINT_DATA_RECORD& back() const;

    POINT_DATA_RECORD&operator[](uint64_t n);
    const POINT_DATA_RECORD&operator[](uint64_t n) const;

protected:
    mutable POINT_DATA_RECORD m_pdr;
    mutable uint64_t m_pointIndex;

private:
    std::function<void(void)> m_fnReadPointDataRecord;

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

    void scalePoint() const;
};


#endif //LIDARVISUALIZER_LASDATASET_H
