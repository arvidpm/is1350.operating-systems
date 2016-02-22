#!/bin/bash

## Save original pwd
olddir=$PWD;

## Declare integer depth and set to 0
declare -i depth=0;

## Tree function
function tree {

        ## Colorization options
        green=`tput setaf 2`
        yellow=`tput setaf 3`
        bold=`tput bold`
        magenta=`tput setaf 5`
        white=`tput setaf 7`
        reset=`tput sgr0`

        # change dir to argument given
        cd "$1";

        ## For every file
        for file in *
        do
                for ((i=0; $i < $depth; i++))
                do
                        ## printf "$i";
                        ## printf "$depth";
                        ## Tab between each level
                        printf "\t";
                        sleep 0.1;
                done

                ## Print directories with brackets ([directory])
                if [[ -d "$file" ]]; then
                        #statements
                        printf "${bold}${yellow}[$file]\n${reset}";
                        sleep 0.1;

                elif [[ -x "$file" ]]; then
                        #statements
                        printf "${bold}${green}$file\n${reset}";
                        sleep 0.1;

                else
                        #statements
                        printf "${white}$file\n${reset}";
                        sleep 0.1;
                fi

                ## Work our way thru the system recursively
                if [[ -d "$file" ]]; then
                        #statements
                        depth=$depth+1;
                        tree "$file";
                        cd ..;
                fi
        done
        ## When done, we move on to the next directory
        let depth=$depth-1;
}

function main() {

        ## Launch tree with specified dir
        tree "$1";

        ## Reset current dir and unset variables
        cd $olddir;
        unset i depth;
}

## Launch main with first argument
main "$1"
