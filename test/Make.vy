make {

   import { Cpp; }

   init {
      $cs := ["basicp.cpp"];
      $ds := [".."];
      $ls := ["vytools"];
      $es := changeExt( $cs, exeExt() );
      Cpp.setIncDir( $ds );
      Cpp.setLibDir( $ds );
      Cpp.setLib( $ds );
      Cpp.setShow(true);
   }

   target {

      /// build and run
      default {
         build();
         run();
      }

      /// build test programs
      build {
         foreach ( c | $cs ) {
            e := changeExt( c, exeExt() );
            if ( older( e, c ))
               Cpp.build( e, c );
         }
      }
         
      /// clean generated files
      clean {
        purge( $es );
      }
   }


}
