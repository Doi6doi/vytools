make {

   init {
      $cs := ["basicp.cpp"];
      $ds := [".."];
      $ls := ["vytoolsp"];
      $es := changeExt( $cs, exeExt() );

      $Cpp := tool("Cpp",{ incDir:$ds, libDir:$ds, 
         lib:$ls, debug:true, earg:"-fsanitize=address" } );
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
               $Cpp.build( e, c );
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
