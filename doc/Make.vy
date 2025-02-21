make {

   import { Dox; }
   
   init {
      $it := "index.txt";
      $rm := "../README.md";
   }

   target {

      /// build documentation
      build {
         readme();
      }

      /// purge generated files
      clean {
         purge( [$rm] );
      }
   }

   function {
      readme() {
         if ( older( $rm, $it ) )
            Dox.build( $rm, $it );
      }
   }

}
