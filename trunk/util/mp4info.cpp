/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is MPEG4IP.
 *
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001-2002.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie     dmackie@cisco.com
 */

#include "util/impl.h"

using namespace mp4v2::util;

extern "C" int main( int argc, char** argv )
{
    const char* const usageString =
        "<file-name>";

    /* begin processing command line */
    char* ProgName = argv[0];
    while ( true ) {
        int c = -1;
        int option_index = 0;
        static const prog::Option long_options[] = {
            { "version", prog::Option::NO_ARG, 0, 'V' },
            { NULL, prog::Option::NO_ARG, 0, 0 }
        };

        c = prog::getOptionSingle( argc, argv, "V", long_options, &option_index );

        if ( c == -1 )
            break;

        switch ( c ) {
            case '?':
                fprintf( stderr, "usage: %s %s\n", ProgName, usageString );
                exit( 0 );
            case 'V':
                fprintf( stderr, "%s - %s\n", ProgName, MP4V2_PROJECT_name_formal );
                exit( 0 );
            default:
                fprintf( stderr, "%s: unknown option specified, ignoring: %c\n",
                         ProgName, c );
        }
    }

    /* check that we have at least one non-option argument */
    if ( ( argc - prog::optind ) < 1 ) {
        fprintf( stderr, "usage: %s %s\n", ProgName, usageString );
        exit( 1 );
    }

    /* end processing of command line */
    printf( "%s version %s\n", ProgName, MP4V2_PROJECT_version );

    while ( prog::optind < argc ) {
        char *mp4FileName = argv[prog::optind++];

        printf( "%s:\n", mp4FileName );

        char* info = MP4FileInfo( mp4FileName );

        if ( !info ) {
            fprintf( stderr,
                     "%s: can't open %s\n",
                     ProgName, mp4FileName );
            continue;
        }

        fputs( info, stdout );
        MP4FileHandle mp4file = MP4Read( mp4FileName ); //, MP4_DETAILS_ERROR);
        if ( mp4file != MP4_INVALID_FILE_HANDLE ) {
            const MP4Tags* tags = MP4TagsAlloc();
            MP4TagsFetch( tags, mp4file );
            char *value;
            uint16_t numvalue, numvalue2;
            if ( tags->name ) {
                fprintf( stdout, " Name: %s\n", tags->name );
            }
            if ( tags->sortName ) {
                fprintf( stdout, " Sort Name: %s\n", tags->sortName );
            }
            if ( tags->artist ) {
                fprintf( stdout, " Artist: %s\n", tags->artist );
            }
            if ( tags->sortArtist ) {
                fprintf( stdout, " Sort Artist: %s\n", tags->sortArtist );
            }
            if ( tags->composer ) {
                fprintf( stdout, " Composer: %s\n", tags->composer );
            }
            if ( tags->sortComposer ) {
                fprintf( stdout, " Sort Composer: %s\n", tags->sortComposer );
            }
            if ( tags->encodingTool ) {
                fprintf( stdout, " Encoded with: %s\n", tags->encodingTool );
            }
            if ( tags->encodedBy ) {
                fprintf( stdout, " Encoded by: %s\n", tags->encodedBy );
            }
            if ( tags->releaseDate ) {
                fprintf( stdout, " Release Date: %s\n", tags->releaseDate );
            }
            if ( tags->album ) {
                fprintf( stdout, " Album: %s\n", tags->album );
            }
            if ( tags->sortAlbum ) {
                fprintf( stdout, " Sort Album: %s\n", tags->sortAlbum );
            }
            if ( MP4GetMetadataTrack( mp4file, &numvalue, &numvalue2 ) ) {
                fprintf( stdout, " Track: %u of %u\n", numvalue,
                         numvalue2 );
            }
            if ( MP4GetMetadataDisk( mp4file, &numvalue, &numvalue2 ) ) {
                fprintf( stdout, " Disk: %u of %u\n", numvalue,
                         numvalue2 );
            }
            if ( tags->genre ) {
                fprintf( stdout, " Genre: %s\n", tags->genre );
            }
            if ( tags->grouping ) {
                fprintf( stdout, " Grouping: %s\n", tags->grouping );
            }
            if ( tags->beatsPerMinute ) {
                fprintf( stdout, " BPM: %u\n", *tags->beatsPerMinute );
            }
            if ( tags->comments ) {
                fprintf( stdout, " Comments: %s\n", tags->comments );
            }
            if ( tags->compilation ) {
                fprintf( stdout, " Part of Compilation: %s\n", *tags->compilation ? "yes" : "no" );
            }
            if ( tags->gapless ) {
                fprintf( stdout, " Part of Gapless Album: %s\n", *tags->gapless ? "yes" : "no" );
            }
            uint32_t artcount = MP4GetMetadataCoverArtCount( mp4file );
            if ( artcount > 0 ) {
                fprintf( stdout, " Cover Art pieces: %u\n",
                         artcount );
            }
            if ( tags->albumArtist ) {
                fprintf( stdout, " Album Artist: %s\n", tags->albumArtist );
            }
            if ( tags->sortAlbumArtist ) {
                fprintf( stdout, " Sort Album Artist: %s\n", tags->sortAlbumArtist );
            }
            if ( tags->copyright ) {
                fprintf( stdout, " Copyright: %s\n", tags->copyright );
            }
            if ( tags->contentRating ) {
                fprintf( stdout, " Explicit Content: %s\n", tags->contentRating ? "yes" : "no" );
            }
            if ( tags->hdVideo ) {
                fprintf( stdout, " HD Video: %s\n", *tags->hdVideo ? "yes" : "no");
            }
            if ( tags->mediaType ) {
                string s = itmf::enumStikType.toString( static_cast<itmf::StikType>( *tags->mediaType ), true );
                fprintf( stdout, " Media Type: %s\n",
                        s.c_str() );
            }
            if ( tags->tvShow ) {
                fprintf( stdout, " TV Show: %s\n", tags->tvShow );
            }
            if ( tags->sortTVShow ) {
                fprintf( stdout, " Sort TV Show: %s\n", tags->sortTVShow );
            }
            if ( tags->tvNetwork ) {
                fprintf( stdout, " TV Network: %s\n", tags->tvNetwork );
            }
            if ( tags->tvEpisodeID ) {
                fprintf( stdout, " TV Episode Number: %s\n", tags->tvEpisodeID );
            }
            if ( tags->description ) {
                fprintf( stdout, " Short Description: %s\n", tags->description );
            }
            if ( tags->longDescription ) {
                fprintf( stdout, " Long Description: %s\n", tags->longDescription );
            }
            if ( tags->lyrics ) {
                fprintf( stdout, " Lyrics: \n%s\n", tags->lyrics );
            }
            if ( tags->tvEpisode ) {
                fprintf( stdout, " TV Episode: %u\n", *tags->tvEpisode );
            }
            if ( tags->tvSeason ) {
                fprintf( stdout, " TV Season: %u\n", *tags->tvSeason );
            }
            if ( tags->podcast) {
                fprintf( stdout, " Podcast: %s\n", *tags->podcast ? "yes" : "no" );
            }
            if ( tags->keywords ) {
                fprintf( stdout, " Keywords: %s\n", tags->keywords );
            }
            if ( tags->category ) {
                fprintf( stdout, " Category: %s\n", tags->category );
            }
            if ( tags->cnID ) {
                fprintf( stdout, " cnID: %u\n", *tags->cnID );
            }
            if ( tags->iTunesAccount ) {
                fprintf( stdout, " iTunes Account: %s\n", tags->iTunesAccount );
            }
            if ( tags->purchaseDate ) {
                fprintf( stdout, " Purchase Date: %s\n", tags->purchaseDate );
            }
            MP4TagsFree( tags );
            MP4Close( mp4file );
        }
        free( info );
    }
    return( 0 );
}
