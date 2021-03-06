:
    eval 'exec perl -S $0 ${1+"$@"}'
        if 0;
#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************


#
# Create ordinary theme from HiContrast images.
#

use File::Copy;
use File::Find;
use File::Path;
use File::Spec;

( $src, $dst ) = @ARGV;

if ( $src eq "" || $dst eq "" ) {
    print STDERR "Usage: hicontrast-to-theme.pl src dest\n\n";
    print STDERR "Create ordinary theme from HiContrast images.\n";
    exit 1;
}

$dst = File::Spec->rel2abs( $dst );

@hc_table = (
    [ ".*_h.png",         "_h.png",    ".png" ],
    [ ".*_sch.png",       "_sch.png",  ".png" ],
    [ ".*_hc.png",        "_hc.png",   ".png" ],
    [ "lch_.*.png",       "lch_",      "lc_" ],
    [ "sch_.*.png",       "sch_",      "sc_" ],
    [ "lch[0-9].*.png",   "lch",       "lc" ],
    [ "sch[0-9].*.png",   "sch",       "sc" ],
    [ "loh[0-9].*.png",   "loh",       "lo" ],
    [ "lxh[0-9].*.png",   "lxh",       "lx" ],
    [ "sxh[0-9].*.png",   "sxh",       "sx" ],
    [ "avh[0-9].*.png",   "avh",       "av" ],
    [ "avlh[0-9].*.png",  "avlh",      "avl" ],
    [ "idh[0-9].*.png",   "idh",       "id" ],
    [ "imh[0-9].*.png",   "imh",       "im" ],
    [ "mih[0-9].*.png",   "mih",       "mi" ],
    [ "tbh[0-9].*.png",   "tbh",       "tb" ],
    [ "nah[0-9].*.png",   "nah",       "na" ],
    [ "nch[0-9].*.png",   "nch",       "nc" ],
    [ "nvh[0-9].*.png",   "nvh",       "nv" ],
    [ "ouh[0-9].*.png",   "ouh",       "ou" ],
    [ "ddh[0-9].*.png",   "ddh",       "dd" ],
    [ "sfh[0-9].*.png",   "sfh",       "sf" ],
    [ "srh[0-9].*.png",   "srh",       "sr" ],
    [ "wrh[0-9].*.png",   "wrh",       "wr" ],
    [ "alh[0-9].*.png",   "alh",       "al" ],
    [ "ath[0-9].*.png",   "ath",       "at" ],
    [ "bih[0-9].*.png",   "bih",       "bi" ],
    [ "coh[0-9].*.png",   "coh",       "co" ],
    [ "foh[0-9].*.png",   "foh",       "fo" ],
    [ "fuh[0-9].*.png",   "fuh",       "fu" ],
    [ "oph[0-9].*.png",   "oph",       "op" ],
    [ "unh[0-9].*.png",   "unh",       "un" ],
    [ "edh[0-9].*.png",   "edh",       "ed" ],
    [ "cdh[0-9].*.png",   "cdh",       "cd" ],
    [ "frh[0-9].*.png",   "frh",       "fr" ],
    [ "fwh[0-9].*.png",   "fwh",       "fw" ],
    [ "nuh[0-9].*.png",   "nuh",       "nu" ],
    [ "prh[0-9].*.png",   "prh",       "pr" ],
    [ "shh[0-9].*.png",   "shh",       "sh" ],
    [ "trh[0-9].*.png",   "trh",       "tr" ],
    [ "reh[0-9].*.png",   "reh",       "re" ],
    [ "joh[0-9].*.png",   "joh",       "jo" ],
    [ "fph[0-9].*.png",   "fph",       "fp" ],
    [ "dah[0-9].*.png",   "dah",       "da" ]
);

my (@from_stat, @to_stat);

sub copy_normalized {
    $file = $_;
    for $hc ( @hc_table ) {
        ( $what, $from, $to ) = @$hc;
        if ( $file =~ /$what/&&!($file=~/\.svn/) ) {
            my $dir = File::Spec->catdir( $dst, $File::Find::dir );

            if ( ! -d $dir ) {
                mkpath( $dir );
            }

            ( my $copy = $file ) =~ s/$from/$to/;
            $copy = File::Spec->catfile( $dir, $copy );

	        @from_stat = stat($file);
	        @to_stat = stat($copy);
			if ( $from_stat[9] > $to_stat[9] ) {
				if ( $^O eq 'os2' ) {
					$rc = unlink($copy);
				}
	            copy( $file, $copy ) || die $!;
				utime( $from_stat[9], $from_stat[9], $copy );
			}

            last;
        }
    }
}

chdir( $src );
find( \&copy_normalized, '.' );
