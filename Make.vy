make {

   init {
      $name := "vytools";
      $url := "https://github.com/Doi6doi/vytools.git";
      $ver := "20250329";
      
      $C := tool( "C", { libMode:true, debug:true, show:true });
      $Cpp := tool( "Cpp", { libMode:true, debug:true, show:true });
      $Git := tool( "Git" );
      
      $cdep := "c.dep";
      $pdep := "p.dep";
      $cs := ["vytools.c"];
      $hs := ["vytools_defs.h","vytools.h","vytools_impl.h"];
      $hps := ["vytools.hpp","vytools_tpl.hpp"];
      $cps := ["vytoolspp.cpp","vyt_str.cpp"];
      case ( system() ) {
         "Linux": $cs += "linux.c";
         "Windows": $cs += "windows.c";
      }
      $ccs := regexp( $cs, "#(.*)\\.c#", "p_\\1.cpp" );
      $lib := $C.libFile( $name );
      $plib := $Cpp.libFile( $name+"p" );
      $purge := ["*"+$C.objExt(), "*"+$Cpp.objExt(), $C.libFile("*"), "*.dep"]
         + $ccs;
   }

   target {

      /// run menu
      menu {
         $Dlg := tool( "Dialog" );
         m := $Dlg.menu("vyTools")
            .item("VyTools is a C and C++ library which"
            +" makes often used tasks easier.");
         if ( inDir() ) {
            m.item("Build libraries",build)
             .item("Clean generated files",clean)
             .item("Generate documentation",docs)
             .item("Run tests",test);
         } else {
            m.item("Download source",download);
         }
         m.exec();
      }

      /// download the project
      download {
         $Git.clone( $url );
         cd("vytools");
      }

     /// build project
      build {
         genCcs();
         genDep();
         genObjs();
         genLibs();
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
            $C.depend( $cdep, $cs );
         if ( older( $pdep, $ccs+$cps+$hs+$hps ) )
            $Cpp.depend( $pdep, $ccs+$cps );
      }

      /// generate object files
      genObjs() {
         ds := $C.loadDep( $cdep );
         foreach ( c | $cs ) {
            o := changeExt( c, $C.objExt() );
            if ( older( o, ds[o] ))
               $C.compile( o, c );
         }
         ds := $Cpp.loadDep( $pdep );
         foreach ( c | $ccs + $cps ) {
            o := changeExt( c, $Cpp.objExt() );
            if ( older( o, ds[o] ))
               $Cpp.compile( o, c );
         }
      }

      /// generate library
      genLibs() {
         os := changeExt( $cs, $C.objExt() );
         if ( older( $lib, os ))
            $C.link( $lib, os );
         pos := changeExt( $cps+$ccs, $Cpp.objExt() );
         if ( older( $plib, pos ))
            $Cpp.link( $plib, pos );
      }

   }

}
