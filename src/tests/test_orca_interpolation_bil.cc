/*
 * (C) Crown Copyright 2022 Met Office
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include <cmath>

#include "eckit/types/FloatCompare.h"

#include "atlas/array.h"
#include "atlas/functionspace.h"
#include "atlas/functionspace/PointCloud.h"
#include "atlas/grid.h"
#include "atlas/interpolation.h"
#include "atlas/mesh.h"
#include "atlas/meshgenerator.h"
#include "atlas/util/CoordinateEnums.h"
#include "atlas/field/MissingValue.h"

#include "tests/AtlasTestEnvironment.h"

using namespace eckit;
using namespace atlas::functionspace;
using namespace atlas::util;

namespace atlas {
namespace test {

//-----------------------------------------------------------------------------
//

CASE("test_interpolation_points_to_ORCA2_bilinear_remapping") {
    Grid grid("ORCA2_T");
    Mesh mesh(grid);
    NodeColumns fs(mesh);

    auto func = [](double x) -> double { return std::sin(x * M_PI / 180.); };

    SECTION("at the equator, after seam") {
        PointCloud pointcloud({{80., 0.},
                               {90., 0.},
                               {100., 0.},
                               {110., 0.},
                               {120., 0.},
                               {130., 0.},
                               {140., 0.},
                               {150., 0.},
                               {160., 0.}});

        Interpolation interpolation(option::type("bilinear-remapping") | util::Config("non_linear", "missing-if-all-missing"), fs, pointcloud);

        Field field_source = fs.createField<double>(option::name("source"));
        field_source.metadata().set("missing_value", -3278.0);
        field_source.metadata().set("missing_value_type", "approximately-equals");
        field_source.metadata().set("missing_value_epsilon", 1e-6);
        Field field_target("target", array::make_datatype<double>(), array::make_shape(pointcloud.size()));
        field_target.metadata().set("missing_value", -3278.0);
        field_target.metadata().set("missing_value_type", "approximately-equals");
        field_target.metadata().set("missing_value_epsilon", 1e-6);

        auto lonlat = array::make_view<double, 2>(fs.nodes().lonlat());
        auto source = array::make_view<double, 1>(field_source);
        for (idx_t j = 0; j < fs.nodes().size(); ++j) {
            source(j) = func(lonlat(j, LON));
        }

        interpolation.execute(field_source, field_target);

        auto target = array::make_view<double, 1>(field_target);

        auto pc_view = array::make_view<double, 2>(pointcloud.lonlat());
        std::vector<double> check;
        for (int i = 0; i < pc_view.size(); ++i) {
            check.push_back(func(pc_view(i, 0)));
        }

        atlas::field::MissingValue mv(field_target);
        std::vector<bool> missing_vals(pointcloud.size(), true);

        for (std::size_t j=0; j < target.size(); ++j) {
          missing_vals[j] = mv(target(j));
        }

        for (idx_t j = 0; j < pointcloud.size(); ++j) {
            static double interpolation_tolerance = 1.e-4;
            char b = missing_vals[j] ? 'T' : 'F';
            Log::info() << target(j) << "  " << check[j] << " " << b << std::endl;
            EXPECT(eckit::types::is_approximately_equal(target(j), check[j], interpolation_tolerance));
            EXPECT(!missing_vals[j]);
        }
    }
    SECTION("at the equator, before seam") {
        PointCloud pointcloud(
            {{00., 0.}, {10., 0.}, {20., 0.}, {30., 0.}, {40., 0.}, {50., 0.}, {60., 0.}, {72., 0.}, {80., 0.}});

        Interpolation interpolation(option::type("bilinear-remapping") | util::Config("non_linear", "missing-if-all-missing"), fs, pointcloud);

        Field field_source = fs.createField<double>(option::name("source"));
        field_source.metadata().set("missing_value", -3278.0);
        field_source.metadata().set("missing_value_type", "approximately-equals");
        field_source.metadata().set("missing_value_epsilon", 1e-6);
        Field field_target("target", array::make_datatype<double>(), array::make_shape(pointcloud.size()));
        field_target.metadata().set("missing_value", -3278.0);
        field_target.metadata().set("missing_value_type", "approximately-equals");
        field_target.metadata().set("missing_value_epsilon", 1e-6);

        auto lonlat = array::make_view<double, 2>(fs.nodes().lonlat());
        auto source = array::make_view<double, 1>(field_source);
        for (idx_t j = 0; j < fs.nodes().size(); ++j) {
            source(j) = func(lonlat(j, LON));
        }

        interpolation.execute(field_source, field_target);

        auto target = array::make_view<double, 1>(field_target);

        auto pc_view = array::make_view<double, 2>(pointcloud.lonlat());
        std::vector<double> check;
        for (int i = 0; i < pc_view.size(); ++i) {
            check.push_back(func(pc_view(i, 0)));
        }

        atlas::field::MissingValue mv(field_target);
        std::vector<bool> missing_vals(pointcloud.size(), true);

        for (std::size_t j=0; j < target.size(); ++j) {
          missing_vals[j] = mv(target(j));
        }

        for (idx_t j = 0; j < pointcloud.size(); ++j) {
            static double interpolation_tolerance = 1.e-4;
            char b = missing_vals[j] ? 'T' : 'F';
            Log::info() << target(j) << "  " << check[j] << " " << b << std::endl;
            EXPECT(eckit::types::is_approximately_equal(target(j), check[j], interpolation_tolerance));
            EXPECT(!missing_vals[j]);
        }
    }
}

//-----------------------------------------------------------------------------

}  // namespace test
}  // namespace atlas

int main(int argc, char** argv) {
    return atlas::test::run(argc, argv);
}
