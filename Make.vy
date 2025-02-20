make {

   import { C; Cpp; Git; }

   function init() {
      $name := "vytools";
      $url := "https://github.com/Doi6doi/vytools.git";
      $cdep := "c.dep";
      $pdep := "p.dep";
      $cs := ["vytools.c"];
      case ( system() ) {
         "Linux": $cs += "linux.c";
         "Windows": $cs += "windows.c";
      }
      $hs := ["vytools_arch.h","vytools.h","vytools_impl.h"];
      $hps := ["vytools.hpp","vytools_tpl.hpp"];
      $cps := ["vytoolsp.cpp","vyt_str.cpp"];
   }

   target {

      /// download and/or build 
      default {
         init();
         if ( inDir() )
            build();
         else {
            download();
            make( $name );
         }
      }

      /// download the project
      download {
         Git.clone( $url );
      }

      // build project
      build {
         init();
         genDep();
         genObjs();
         genLib();
         echo("BUILD");
      }

      // clear generated files
      clean {
        init();
        purge( ["*"+C.objExt(), "*"+Cpp.objExt(), C.libFile("*")] );
      }

      // run tests
      test {
         make( "test" );
      }
   }

   function {
      /// are we in project directory
      inDir() {
         return exists("vytools.h");
      }

      /// generate dependency files
      genDep() {
         if ( older( $cdep, $cs+$hs ) )
            C.depend( $cdep, $cs );
         if ( older( $pdep, $cps+$hs+$hps ) )
            Cpp.depend( $pdep, $cps );
      }


      /// generate object files
      genObjs() {
         ds := C.loadDep( $cdep );
         foreach ( c | $cs ) {
            o := changeExt(c,C.objExt());
            if ( older( o, ds[o] ))
               C.compile( o, c );
         }
         ds := Cpp.loadDep( $pdep );
         foreach ( c | $cps ) {
            o := changeExt(c,Cpp.objExt());
            if ( older( o, ds[o] ))
               Cpp.compile( o, c );
         }
      }

   }

}
