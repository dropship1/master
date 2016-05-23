#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.h"
#include "base/actor.hpp"

SCENARIO( "actor's public interface should work as designed", "[actor]" ) {

  GIVEN( "A shared pointer to a default constructed actor" ) {
    auto pActor = std::make_shared<bright::base::Actor>();

    REQUIRE( pActor->has_children() == false );
    REQUIRE( pActor->is_root() == false );
    REQUIRE( pActor->name().compare("default") == 0 );
    REQUIRE( pActor->unique_id() == 0 );
    REQUIRE( pActor->children().size() == 0 );
 
    WHEN( "the name is set" ) {
      pActor->name( "newName!" );

      THEN( "the name is different" ) {
        REQUIRE( pActor->name().compare("newName!") == 0 );
      }
    }
    WHEN( "the root is set" ) {
      pActor->is_root(true);

      THEN( "root is different" ) {
        REQUIRE( pActor->is_root() == true );
      }
    }
    WHEN( "the unique id is set" ) {
      pActor->unique_id(1);

      THEN( "unique id is different" ) {
        REQUIRE( pActor->unique_id() == 1 );
      }
    }

    WHEN( "a child is added" ) {
      std::shared_ptr<bright::base::Actor> pChild = std::make_shared<bright::base::Actor>();
      pChild->name("child");
      REQUIRE( pChild->name().compare("child") == 0 );
      pActor->add_child(pChild);
      REQUIRE( pChild.use_count() == 1 );

      THEN( "has children is true" ) {
        REQUIRE( pActor->has_children() == true );
      }

      THEN( "children size is 1" ) {
        REQUIRE( pActor->children().size() == 1 );
      }
    }

  }

}


SCENARIO( "actor shared pointer should work as expected", "[actor]" ) {

  GIVEN( "A shared pointer to a default constructed actor that has been modified to be unique and to have 1 child" ) {
    std::shared_ptr<bright::base::Actor> pActor1 = std::make_shared<bright::base::Actor>();
 
    REQUIRE( pActor1->has_children() == false );
    REQUIRE( pActor1->is_root() == false );
    REQUIRE( pActor1->name().compare("default") == 0 );
    REQUIRE( pActor1->unique_id() == 0 );
    REQUIRE( pActor1->children().size() == 0 );

    //Verify the shared pointer is working as expected
    REQUIRE( pActor1.use_count() == 1 );

    pActor1->unique_id(1);
    REQUIRE( pActor1->unique_id() == 1 );

    pActor1->is_root(true);
    REQUIRE( pActor1->is_root() == true );

    std::shared_ptr<bright::base::Actor> pChild = std::make_shared<bright::base::Actor>();
    pChild->name("child");
    pChild->unique_id(2);
    REQUIRE( pChild->name().compare("child") == 0 );
    REQUIRE( pChild->unique_id() == 2 );
    REQUIRE( pChild->has_children() == false );
    REQUIRE( pChild->is_root() == false );
    pActor1->add_child(pChild);
    REQUIRE( pChild.use_count() == 1 );


    WHEN( "pActor1 is cloned to pActor2" ) {
      std::shared_ptr<bright::base::Actor> pActor2 = pActor1->clone();

      THEN( "the actors should match, except unique id should be 0 for pActor and pChild" ) {
        REQUIRE( pActor2.has_children() == pActor1.has_children() );
        REQUIRE( pActor2.is_root() == pActor1.is_root() );
        REQUIRE( pActor2.name().compare( pActor1.name() ) == 0 );
        REQUIRE( pActor2.unique_id() == 0 );
        REQUIRE( pActor2.children().size() == pActor1.children().size() );
      }
    }
    WHEN( "the root is set" ) {
      pActor.is_root(true);

      THEN( "root is different" ) {
        REQUIRE( pActor.is_root() == true );
      }
    }
    WHEN( "the unique id is set" ) {
      pActor.unique_id(1);

      THEN( "unique id is different" ) {
        REQUIRE( pActor.unique_id() == 1 );
      }
    }

  }

}




SCENARIO( "actor's shared pointer children should live and die as expected", "[actor]" ) {

  GIVEN( "A default constructed actor that has been modified" ) {
    std::shared_ptr<bright::base::Actor> pActor1 = std::make_shared<bright::base::Actor>();
 
    REQUIRE( pActor->has_children() == false );
    REQUIRE( pActor->is_root() == false );
    REQUIRE( pActor->name().compare("default") == 0 );
    REQUIRE( pActor->unique_id() == 0 );

    WHEN( "pActor1 is copied via assignment to pActor2" ) {
      std::shared_ptr<bright::base::Actor> pActor2 = std::make_shared<bright::base::Actor>(pActor1);

      THEN( "the actor should match" ) {
        REQUIRE( pActor1.name().compare( pActor2.name() ) == 0 );
        REQUIRE( pActor1.unique_id() == pActor2.unique_id() );
        REQUIRE( pActor1.is_root() == pActor2.is_root() );
      }
    }
    WHEN( "the root is set" ) {
      pActor.is_root(true);

      THEN( "root is different" ) {
        REQUIRE( pActor.is_root() == true );
      }
    }
    WHEN( "the unique id is set" ) {
      pActor.unique_id(1);

      THEN( "unique id is different" ) {
        REQUIRE( pActor.unique_id() == 1 );
      }
    }

  }

}