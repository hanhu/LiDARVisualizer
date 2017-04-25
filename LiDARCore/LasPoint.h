//
// Created by Han Hu on 3/11/17.
//

#ifndef LIDARVISUALIZER_LASPOINT_H
#define LIDARVISUALIZER_LASPOINT_H

#include <stdlib.h>
#include <cmath>

class LasQuantizer {
public:
    LasQuantizer();

    LasQuantizer(const double xScale, const double yScale, const double zScale,
    const double xOffset, const double yOffset, const double zOffset);

    double m_xScale;
    double m_yScale;
    double m_zScale;
    double m_xOffset;
    double m_yOffset;
    double m_zOffset;

    inline double getXCoordinate(const int32_t X) const {return X * m_xScale + m_xOffset;}
    inline double getYCoordinate(const int32_t Y) const {return Y * m_yScale + m_yOffset;}
    inline double getZCoordinate(const int32_t Z) const {return Z * m_zScale + m_zOffset;}

    inline int32_t getXRecord(const double X) const {return std::round((X - m_xOffset)/m_xScale); }
    inline int32_t getYRecord(const double Y) const {return std::round((Y - m_yOffset)/m_yScale); }
    inline int32_t getZRecord(const double Z) const {return std::round((Z - m_zOffset)/m_zScale); }
};

struct WAVE_PACKET{
    uint8_t wavePacketDescriptionIndex;
    uint64_t byteOffsetToWaveformData;
    uint32_t waveformPacketSizeInBytes;
    float    returnPointWaveformLocation;
    float    X_t;
    float    Y_t;
    float    Z_t;
};

struct POINT_DATA_RECORD_FORMAT_0 {
    int32_t X;
    int32_t Y;
    int32_t Z;
    uint16_t intensity;
    uint8_t returnNumber:3;
    uint8_t numberOfReturns:3;
    uint8_t scanDirectionFlag:1;
    uint8_t edgeOfFlightLine:1;
    uint8_t classification:5;
    uint8_t  syntheticFlag : 1;
    uint8_t  keypointFlag : 1;
    uint8_t  withheldFlag : 1;
    char    scanAngleRank;
    uint8_t userData;
    uint16_t pointSourceID;
};

struct POINT_DATA_RECORD_FORMAT_6 {
    int32_t X;
    int32_t Y;
    int32_t Z;
    uint16_t intensity;
    uint8_t  extendedReturnNumber:4;
    uint8_t  extendedNumberOfReturns:4;
    uint8_t  classificationFlags:4;
    uint8_t  scanChannel:2;
    uint8_t scanDirectionFlag:1;
    uint8_t edgeOfFlightLine:1;
    uint8_t extendedClassification;
    uint8_t userData;
    int16_t extendedScanAngle;
    uint16_t pointSourceID;
    double GPSTime;
};

class LasPoint {
public:
    union {
        POINT_DATA_RECORD_FORMAT_0 record0;
        POINT_DATA_RECORD_FORMAT_6 record6;
    } basicPointData;

    // Point data record format 1
    double   GPSTime;

    // Point data record format 2
    uint16_t red;
    uint16_t  green;
    uint16_t  blue;

    WAVE_PACKET wavePacket;

    uint16_t  NIR;

    uint8_t m_pointDataRecordFormat;
    LasQuantizer m_lasQuantizer;

    // Getters
    inline uint8_t getPointDataRecordFormat() const { return m_pointDataRecordFormat; }

    inline int32_t getX() const { return basicPointData.record0.X; }
    inline int32_t getY() const { return basicPointData.record0.Y; }
    inline int32_t getZ() const { return basicPointData.record0.Z; }
    inline uint16_t getIntensity() const { return basicPointData.record0.intensity; }

    inline double getXCoordinate() const { return m_lasQuantizer.getXCoordinate(getX()); }
    inline double getYCoordinate() const { return m_lasQuantizer.getYCoordinate(getY()); }
    inline double getZCoordinate() const { return m_lasQuantizer.getZCoordinate(getZ()); }

    inline uint8_t getReturnNumber() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.returnNumber;
        return basicPointData.record6.extendedReturnNumber;
    }

    inline uint8_t  getNumberOfReturns() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.numberOfReturns;
        return basicPointData.record6.extendedNumberOfReturns;
    }

    inline uint8_t getScanDirectionFlag() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.scanDirectionFlag;
        return basicPointData.record6.scanDirectionFlag;
    }

    inline uint8_t getEdgeOfFlightLine() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.edgeOfFlightLine;
        return basicPointData.record6.edgeOfFlightLine;
    }

    inline uint8_t getClassification() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.classification;
        return basicPointData.record6.extendedClassification;
    }

    inline uint8_t getUserData() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.userData;
        return basicPointData.record6.userData;
    }

    inline int16_t getScanAngle() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.scanAngleRank;
        return basicPointData.record6.extendedScanAngle;
    }

    inline uint16_t getPointSourceID() const {
        if (getPointDataRecordFormat() < 6)
            return basicPointData.record0.pointSourceID;
        return basicPointData.record6.pointSourceID;
    }

    inline double getGPSTime() const {
        if (getPointDataRecordFormat() > 6)
            return basicPointData.record6.GPSTime;
        return this->GPSTime;
    }

    void init(const uint8_t pointDataRecordFormat, const LasQuantizer lasQuantizer);
};


#endif //LIDARVISUALIZER_LASPOINT_H
