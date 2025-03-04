make {

   import { Cpp; }
 
   init {
      $cs := ["basicp.cpp"];
      $ds := [".."];
      $ls := ["vytoolsp"];
      $es := changeExt( $cs, exeExt() );
      Cpp.setCompiler("clang");
      Cpp.set( { incDir:$ds, libDir:$ds, 
         lib:$ls, show:true,debug:true } );
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
         
      /// run test programs
      run {
         setPath( $ds+["."] );
         foreach ( e | $es )
            exec( e );
      }
         
      /// clean generated files
      clean {
        purge( $es );
      }
   }

}
