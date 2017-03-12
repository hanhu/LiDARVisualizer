//
// Created by Han Hu on 3/11/17.
//

#include "LasPoint.h"

LasQuantizer::LasQuantizer()
: m_xScale(0.01), m_yScale(0.01), m_zScale(0.01),
  m_xOffset(0.0), m_yOffset(0.0), m_zOffset(0.01)
{

}

LasQuantizer::LasQuantizer(const double xScale, const double yScale, const double zScale, const double xOffset,
                           const double yOffset, const double zOffset)
: m_xScale(xScale), m_yScale(yScale), m_zScale(zScale), m_xOffset(xOffset), m_yOffset(yOffset), m_zOffset(zOffset) {

}

void LasPoint::init(const uint8_t pointDataRecordFormat, const LasQuantizer lasQuantizer) {
    this->m_pointDataRecordFormat = pointDataRecordFormat;
    this->m_lasQuantizer = lasQuantizer;
}