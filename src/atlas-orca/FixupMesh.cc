
#include "FixupMesh.h"
#include "FixupMeshUtils.h"

#include "FixupMesh_ORCA2.h"
#include "FixupMesh_eORCA1.h"

#include "OrcaGrid.h"
#include "atlas/mesh.h"

namespace atlas {
namespace meshgenerator {

//----------------------------------------------------------------------------------------------------------------------

namespace {
template <class T, class... Args>
std::unique_ptr<T> make_unique( Args&&... args ) {
    return std::unique_ptr<T>( new T( std::forward<Args>( args )... ) );
}
}  // namespace

std::unique_ptr<FixupMesh> FixupMesh::create( const util::Config& config ) {
    std::string type = config.getString( "type" );
    if ( type == "ORCA2_T" ) {
        return make_unique<orca::Fixup_ORCA2_T>( config );
    }
    if ( type == "ORCA2_F" ) {
        return make_unique<orca::Fixup_ORCA2_F>( config );
    }
    if ( type == "ORCA2_U" ) {
        return make_unique<orca::Fixup_ORCA2_U>( config );
    }
    if ( type == "ORCA2_V" ) {
        return make_unique<orca::Fixup_ORCA2_V>( config );
    }
    if ( type == "ORCA2_W" ) {
        return make_unique<orca::Fixup_ORCA2_W>( config );
    }
    if ( type == "eORCA1_T" ) {
        return make_unique<orca::Fixup_eORCA1_T>( config );
    }
    if ( type == "eORCA1_F" ) {
        return make_unique<orca::Fixup_eORCA1_F>( config );
    }
    if ( type == "eORCA1_U" ) {
        return make_unique<orca::Fixup_eORCA1_U>( config );
    }
    if ( type == "eORCA1_V" ) {
        return make_unique<orca::Fixup_eORCA1_V>( config );
    }
    if ( type == "eORCA1_W" ) {
        return make_unique<orca::Fixup_eORCA1_W>( config );
    }
    return make_unique<orca::FixupMesh_ORCA>( config );
}

void FixupMesh::execute( Mesh& mesh ) const {
    MeshBuilder mesh_builder( mesh );
    execute( mesh_builder );
}

namespace orca {
FixupMesh_ORCA::FixupMesh_ORCA( const eckit::Parametrisation& config ) {
    config.get( "include_south_pole", include_south_pole_ );
}

void FixupMesh_ORCA::execute( MeshBuilder& mesh_builder ) const {
    mesh_builder.mesh.metadata().set( "valid_elements", false );
}

}  // namespace orca

//----------------------------------------------------------------------------------------------------------------------

}  // namespace meshgenerator
}  // namespace atlas
