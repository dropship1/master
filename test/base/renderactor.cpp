#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.h"
#include "base/renderactor.hpp"


SCENARIO( "renderactor's public interface should work as designed", "[renderactor]" ) {

  GIVEN( "A shared pointer to a default constructed renderactor" ) {
    auto pRenderActor = std::make_shared<bright::base::RenderActor>();

    REQUIRE( pRenderActor->render_name().compare("") == 0 );
    REQUIRE( pRenderActor->mesh().compare("") == 0 );
    REQUIRE( pRenderActor->shader().compare("") == 0 );
    REQUIRE( pRenderActor->camera_type().compare("") == 0 );
 
    WHEN( "the name is set" ) {
      pRenderActor->render_name( "newName!" );

      THEN( "the name is different" ) {
        REQUIRE( pRenderActor->render_name().compare("newName!") == 0 );
      }
    }
    WHEN( "the mesh is set" ) {
      pRenderActor->mesh("newMesh!");

      THEN( "mesh is different" ) {
        REQUIRE( pRenderActor->mesh().compare("newMesh!") == 0 );
      }
    }
    WHEN( "the shader is set" ) {
      pRenderActor->shader("newShader!");

      THEN( "shader is different" ) {
        REQUIRE( pRenderActor->shader().compare("newShader!") == 0 );
      }
    }
    WHEN( "the camera_type is set" ) {
      pRenderActor->camera_type("newCameraType!");

      THEN( "camera_type is different" ) {
        REQUIRE( pRenderActor->camera_type().compare("newCameraType!") == 0 );
      }
    }

  }

}


