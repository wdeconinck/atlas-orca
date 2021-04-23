/*
 * (C) Copyright 2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <array>

#include "eckit/filesystem/PathName.h"

#include "atlas/array/DataType.h"
#include "atlas/util/Config.h"

namespace atlas {
namespace orca {

//------------------------------------------------------------------------------------------------------

class OrcaData {
public:
    std::array<std::int32_t,2> dimensions{-1,-1};
    std::array<std::int32_t,4> halo{-1,-1,-1,-1};
    std::array<double,2> pivot{-1,-1};
    std::vector<double> lon;
    std::vector<double> lat;
    std::vector<std::byte> flags;

    void checkSetup();

    size_t write( const eckit::PathName& path, const util::Config& config );

    size_t detectInvalidElementss( const util::Config& config );

    std::string computeUid( const util::Config& config );

    void setGhost();
};

//------------------------------------------------------------------------------------------------------

}  // namespace orca
}  // namespace atlas
