make {

   import { C; Cpp; Git; }

   init {
      $name := "vytools";
      $url := "https://github.com/Doi6doi/vytools.git";
      $cdep := "c.dep";
      $pdep := "p.dep";
      $cs := ["vytools.c"];
      $hs := ["vytools_arch.h","vytools.h","vytools_impl.h"];
      $hps := ["vytools.hpp","vytools_tpl.hpp"];
      $cps := ["vytoolspp.cpp","vyt_str.cpp"];
      case ( system() ) {
         "Linux": $cs += "linux.c";
         "Windows": $cs += "windows.c";
      }
      $ccs := regexp( $cs, "#(.*)\\.c#", "p_\\1.cpp" );
      $lib := C.libFile( $name );
      $plib := Cpp.libFile( $name+"p" );
      $purge := ["*"+C.objExt(), "*"+Cpp.objExt(), C.libFile("*"), "*.dep"]
         + $ccs;
      C.set( "libMode", true );
      Cpp.set( "libMode", true );
      C.set( "debug", true);
      Cpp.set( "debug", true);
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
         genCcs();
         genDep();
         genObjs();
         genLib();
      }

      /// clear generated files
      clean {
        purge( $purge );
      }

      /// create documentation
      docs {
         make("docs");
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

      /// generate copy of .c files for c++ compiling
      genCcs() {
         foreach ( c | $cs ) {
            cc := regexp( c, "#(.*)\\.c#", "p_\\1.cpp" );
            if ( older( cc, c ))
               copy( c, cc );
         }
      }

      /// generate dependency files
      genDep() {
         if ( older( $cdep, $cs+$hs ) )
            C.depend( $cdep, $cs );
         if ( older( $pdep, $ccs+$cps+$hs+$hps ) )
            Cpp.depend( $pdep, $ccs+$cps );
      }

      /// generate object files
      genObjs() {
         ds := C.loadDep( $cdep );
         foreach ( c | $cs ) {
            o := changeExt( c, C.objExt() );
            if ( older( o, ds[o] ))
               C.compile( o, c );
         }
         ds := Cpp.loadDep( $pdep );
         foreach ( c | $ccs + $cps ) {
            o := changeExt( c, Cpp.objExt() );
            if ( older( o, ds[o] ))
               Cpp.compile( o, c );
         }
      }

      /// generate library
      genLib() {
         os := changeExt( $cs, C.objExt() );
         if ( older( $lib, os ))
            C.link( $lib, os );
         pos := changeExt( $cps+$ccs, Cpp.objExt() );
         if ( older( $plib, pos ))
            Cpp.link( $plib, pos );
      }

   }

}
