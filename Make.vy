make {

   import { C; Cpp; Git; }

   init {
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
      $os := changeExt( $cs+$cps, Cpp.objExt() );
      $lib := Cpp.libFile( $name );
      C.setLibMode( true );
      Cpp.setLibMode( true );
      C.setDebug(true);
      Cpp.setDebug(true);
   }

   target {

      /// download and/or build 
      default {
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

      /// build project
      build {
         genDep();
         genObjs();
         genLib();
      }

      /// clear generated files
      clean {
        purge( ["*"+C.objExt(), "*"+Cpp.objExt(), C.libFile("*"), "*.dep"] );
      }

      /// create documentation
      doc {
         make("doc");
      }

      /// run tests
      test {
         make("test");
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

      /// generate library
      genLib() {
         if ( older( $lib, $os ))
            Cpp.link( $lib, $os );
      }

   }

}
