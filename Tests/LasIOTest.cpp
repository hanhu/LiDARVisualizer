//
// Created by Han Hu on 3/7/17.
//

#include <gtest/gtest.h>
#include "lasreader.hpp"
#include "lasreader.h"
#include "LasWriter.h"

using namespace std;

void compareLasPublicHeaderBlock(LASreader* base, LasReader test) {
        for (size_t i = 0; i < 4; i++)
        EXPECT_EQ(base->header.file_signature[i], test.getPublicHeaderBlock().fileSignature[i]);
        EXPECT_EQ(base->header.file_source_ID, test.getPublicHeaderBlock().fileSourceID);
        EXPECT_EQ(base->header.global_encoding, test.getPublicHeaderBlock().globalEncoding);
        EXPECT_EQ(base->header.project_ID_GUID_data_1, test.getPublicHeaderBlock().projectID_GUIDData1);
        EXPECT_EQ(base->header.project_ID_GUID_data_2, test.getPublicHeaderBlock().projectID_GUIDData2);
        EXPECT_EQ(base->header.project_ID_GUID_data_3, test.getPublicHeaderBlock().projectID_GUIDdata3);
        for (size_t i = 0; i < 8; i++)
        EXPECT_EQ(base->header.project_ID_GUID_data_4[i], test.getPublicHeaderBlock().projectID_GUIDData4[i]);
        EXPECT_EQ(base->header.version_major, test.getPublicHeaderBlock().versionMajor);
        EXPECT_EQ(base->header.version_minor, test.getPublicHeaderBlock().versionMinor);
        EXPECT_STREQ(base->header.system_identifier, (char*)test.getPublicHeaderBlock().systemIdentifier);
        EXPECT_STREQ(base->header.generating_software, (char*)test.getPublicHeaderBlock().generatingSoftware);
        EXPECT_EQ(base->header.file_creation_day, test.getPublicHeaderBlock().fileCreationDayOfYear);
        EXPECT_EQ(base->header.file_creation_year, test.getPublicHeaderBlock().fileCreationYear);
        EXPECT_EQ(base->header.header_size, test.getPublicHeaderBlock().headerSize);
        ASSERT_EQ(base->header.offset_to_point_data, test.getPublicHeaderBlock().offsetToPointData);
        ASSERT_EQ(base->header.number_of_variable_length_records, test.getPublicHeaderBlock().numberOfVariableLengthRecords);
        EXPECT_EQ(base->header.point_data_format, test.getPublicHeaderBlock().pointDataRecordFormat);
        EXPECT_EQ(base->header.point_data_record_length, test.getPublicHeaderBlock().pointDataRecordLength);
        EXPECT_EQ(base->header.number_of_point_records, test.getPublicHeaderBlock().numberOfPointRecords);
        for (size_t i = 0; i < 5; i++)
        EXPECT_EQ(base->header.number_of_points_by_return[i], test.getPublicHeaderBlock().numberOfPointsByReturn[i]);
        EXPECT_EQ(base->header.x_scale_factor, test.getPublicHeaderBlock().xScaleFactor);
        EXPECT_EQ(base->header.y_scale_factor, test.getPublicHeaderBlock().yScaleFactor);
        EXPECT_EQ(base->header.z_scale_factor, test.getPublicHeaderBlock().zScaleFactor);
        EXPECT_EQ(base->header.x_offset, test.getPublicHeaderBlock().xOffset);
        EXPECT_EQ(base->header.y_offset, test.getPublicHeaderBlock().yOffset);
        EXPECT_EQ(base->header.z_offset, test.getPublicHeaderBlock().zOffset);
        EXPECT_EQ(base->header.max_x, test.getPublicHeaderBlock().maxX);
        EXPECT_EQ(base->header.min_x, test.getPublicHeaderBlock().minX);
        EXPECT_EQ(base->header.max_y, test.getPublicHeaderBlock().maxY);
        EXPECT_EQ(base->header.min_y, test.getPublicHeaderBlock().minY);
        EXPECT_EQ(base->header.max_z, test.getPublicHeaderBlock().maxZ);
        EXPECT_EQ(base->header.min_z, test.getPublicHeaderBlock().minZ);
};

void comparePointData(LASreader* lasreader_base, LasReader lasreader_test) {
    uint64_t countOfPoints = 0;
    for (LasReader::const_iterator it = lasreader_test.begin(); it < lasreader_test.end(); it++) {
        lasreader_base->read_point();

        ASSERT_EQ(lasreader_base->point.X, it->getX());
        ASSERT_EQ(lasreader_base->point.Y, it->getY());
        ASSERT_EQ(lasreader_base->point.Z, it->getZ());
        ASSERT_EQ(lasreader_base->point.intensity, it->getIntensity());
        ASSERT_EQ(lasreader_base->point.return_number, it->getReturnNumber());
        ASSERT_EQ(lasreader_base->point.number_of_returns, it->getNumberOfReturns());
        ASSERT_EQ(lasreader_base->point.scan_direction_flag, it->getScanDirectionFlag());
        ASSERT_EQ(lasreader_base->point.edge_of_flight_line, it->getEdgeOfFlightLine());
        ASSERT_EQ(lasreader_base->point.classification, it->getClassification());
        ASSERT_EQ(lasreader_base->point.scan_angle_rank, it->getScanAngle());
        ASSERT_EQ(lasreader_base->point.user_data, it->getUserData());
        ASSERT_EQ(lasreader_base->point.point_source_ID, it->getPointSourceID());
        ASSERT_EQ(lasreader_base->point.gps_time, it->getGPSTime());

        countOfPoints++;
    }
    EXPECT_EQ(lasreader_base->header.number_of_point_records, countOfPoints);
}

TEST(IOTEST, readtest)
{
    const char* fileName = "NS_001_SW_PtCl.las";

    // lastools lasreader
    LASreadOpener lasreadopener;
    lasreadopener.set_file_name(fileName);
    LASreader* lasreader_base = lasreadopener.open();

    // homebrew lasreader
    LasReader lasreader_test;
    lasreader_test.open(fileName);

    // Compare header
    compareLasPublicHeaderBlock(lasreader_base, lasreader_test);

    // Compare point data
    comparePointData(lasreader_base, lasreader_test);

    // Close lastools
    lasreader_base->close();
    delete lasreader_base;

    // Close homebrew lasreader
    lasreader_test.close();
}

TEST(IOTEST, writetest)
{
    const char* fileName = "NS_001_SW_PtCl.las";
    const char* outputFileName = "write_test.las";

    LasReader lasReader;
    lasReader.open(fileName);

    LasWriter lasWriter;
    lasWriter.create(outputFileName, lasReader.getPublicHeaderBlock(), lasReader.getVariableLengthRecords(), lasReader.getExtendedVariableLengthRecords());

    uint64_t countOfPoints = 0;
    for (LasReader::const_iterator it = lasReader.begin(); it < lasReader.end(); it++) {
        lasWriter.writePointDataRecord(*it);
        countOfPoints++;
    }
    lasWriter.close();
    lasReader.close();

    // Compare header
    FILE* fp1 = fopen(fileName, "r"), * fp2 = fopen(outputFileName, "r");
    // compare header
    auto headerSize = lasReader.getPublicHeaderBlock().headerSize;
    for (auto i = 0; i < headerSize; i++)
    {
        int c1 = fgetc(fp1);
        int c2 = fgetc(fp2);
        ASSERT_EQ(c1, c2);
    }
    auto offsetToPointData = lasReader.getOffsetToPointData();
    fseek(fp1, offsetToPointData, SEEK_SET);
    fseek(fp2, offsetToPointData, SEEK_SET);
    auto pointDataSize = lasReader.getPointDataRecordLength() * lasReader.getNumberOfPointRecords();
    for (auto i = 0; i < pointDataSize; i++) {
        auto c1 = fgetc(fp1);
        auto c2 = fgetc(fp2);
        ASSERT_EQ(c1, c2);
    }
    fclose(fp1);
    fclose(fp2);
}