make {

   import { Git; }

   target {
      // download the project
      download {
         Git.clone( "https://github.com/Doi6doi/vytools.git" );
      }
   }

}
