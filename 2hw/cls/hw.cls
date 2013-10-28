\NeedsTeXFormat{LaTeX2e}[1994/06/01]

\ProvidesClass{homework}[2009/05/15 v0.0.1 Document class for typesetting
homework]

\LoadClass[12pt,letterpaper,answers]{exam}
\RequirePackage{xspace}
\newcommand{\course}[1]{\def\@course{#1\xspace}}
\newcommand{\problemlist}[1]{\begin{center}\large #1 \end{center}}

\makeatletter
\renewcommand{\maketitle}{
\newpage
 \null
 \firstpageheader{}{}{}
 \runningheader{\@author}{\@course \@title}{\@date}
 \begin{flushright}
  \bf\large \@author \\ \@course \\ \@title \\ \@date
 \end{flushright}
} \makeatother

\RequirePackage[margin=1in]{geometry}
\RequirePackage{amsmath}
\RequirePackage{amssymb}
\RequirePackage{amsthm}
\RequirePackage[usenames,dvipsnames]{color}
\RequirePackage{todonotes}
\RequirePackage{graphicx}
\RequirePackage{url}
\RequirePackage{listings}

\RequirePackage{tikz}
\usetikzlibrary{arrows,snakes,patterns}

\RequirePackage{siunitx}
%\newunit{\inch}{in}
%\newunit{\cycles}{cycles}
\sisetup{load=accepted,repeatunits=false}

\RequirePackage[version=3]{mhchem}

\RequirePackage{hyperref}

% LaTeX settings for MATLAB code listings
% based on Ted Pavlic's settings in
% http://links.tedpavlic.com/ascii/homework_new_tex.ascii
% This is the color used for MATLAB comments below
\definecolor{MyDarkGreen}{rgb}{0.0,0.4,0.0}

% For faster processing, load Matlab syntax for listings
\lstloadlanguages{Matlab}%
\lstset{language=Matlab,                        % Use MATLAB
        frame=single,                           % Single frame around code
        basicstyle=\small\ttfamily,             % Use small true type font
        keywordstyle=[1]\color{Blue}\bfseries,        % MATLAB functions bold
and blue
        keywordstyle=[2]\color{Purple},         % MATLAB function
arguments purple
        keywordstyle=[3]\color{Blue}\underbar,  % User functions
underlined and blue
        identifierstyle=,                       % Nothing special about
identifiers
                                                % Comments small dark
                                                % green courier
        commentstyle=\usefont{T1}{pcr}{m}{sl}\color{MyDarkGreen}\small,
        stringstyle=\color{Purple},             % Strings are purple
        showstringspaces=false,                 % Don't put marks in
string spaces
        tabsize=5,                              % 5 spaces per tab
        %
        %%% Put standard MATLAB functions not included in the default
        %%% language here
        morekeywords={xlim,ylim,var,alpha,factorial,importdata,poissrnd,normpdf,normcdf,strcat},
        %
        %%% Put MATLAB function parameters here
        morekeywords=[2]{on, off, interp},
        %
        %%% Put user defined functions here
        morekeywords=[3]{FindESS, homework_example},
        %
        morecomment=[l][\color{Blue}]{...},     % Line continuation (...) like
blue comment
        numbers=left,                           % Line numbers on left
        firstnumber=1,                          % Line numbers start with line
1
        numberstyle=\tiny\color{Blue},          % Line numbers are blue
        stepnumber=5                            % Line numbers go in steps of 5
        }

% Includes a MATLAB script.
% The first parameter is the label, which also is the name of the script
% without the .m.
% The second parameter is the optional caption.
\newcommand{\matlabscript}[2]
  {\begin{itemize}\item[]\lstinputlisting[caption=#2,label=#1]{#1.m}\end{itemize}}

\newcommand{\real}{\operatorname{real}}
\newcommand{\imag}{\operatorname{imag}}
\newcommand{\abs}[1]{\ensuremath{\left\lvert #1 \right\rvert}}
\newcommand{\Arg}{\operatorname{Arg}}
\newcommand{\Log}{\operatorname{Log}}
\newcommand{\Res}{\operatornamewithlimits{Res}}
\renewcommand{\vec}[1]{\ensuremath{\textbf{#1}}}
\newcommand{\sumover}[1]{\ensuremath{\sum_{#1=-\infty}^\infty}}

\theoremstyle{definition}
\newtheorem*{claim}{Claim}
\newtheorem*{theorem}{Theorem}

\newcommand{\sech}{\operatorname{sech}}
\newcommand{\rect}{\operatorname{rect}}
\newcommand{\sinc}{\operatorname{sinc}}
\newcommand{\jinc}{\operatorname{jinc}}
\newcommand{\MATLAB}{{\scshape Matlab}}
\renewcommand{\qedsymbol}{$\blacksquare$}
\newcommand{\figbox}{\framebox[3in]{\rule{0pt}{2in}}}

\renewcommand{\theenumi}{\alph{enumi}}
