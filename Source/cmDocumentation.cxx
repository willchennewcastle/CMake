/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmDocumentation.h"

#include "cmSystemTools.h"

//----------------------------------------------------------------------------
static const cmDocumentationEntry cmDocumentationStandardOptions[] =
{
  {"--copyright [file]", "Print the CMake copyright and exit.",
   "If a file is specified, the copyright is written into it."},
  {"--help", "Print usage information and exit.",
   "Usage describes the basic command line interface and its options."},
  {"--help-command cmd [file]", "Print help for a single command and exit.",
   "Full documentation specific to the given command is displayed."},
  {"--help-command-list [file]", "List available listfile commands and exit.",
   "The list contains all commands for which help may be obtained by using "
   "the --help-command argument followed by a command name.  If a file is "
   "specified, the help is written into it."},
  {"--help-full [file]", "Print full help and exit.",
   "Full help displays most of the documentation provided by the UNIX "
   "man page.  It is provided for use on non-UNIX platforms, but is "
   "also convenient if the man page is not installed.  If a file is "
   "specified, the help is written into it."},
  {"--help-html [file]", "Print full help in HTML format.",
   "This option is used by CMake authors to help produce web pages.  "
   "If a file is specified, the help is written into it."},
  {"--help-man [file]", "Print a UNIX man page and exit.",
   "This option is used by the cmake build to generate the UNIX man page.  "
   "If a file is specified, the help is written into it."},
  {"--version [file]", "Show program name/version banner and exit.",
   "If a file is specified, the version is written into it."},
  {0,0,0}
};

//----------------------------------------------------------------------------
static const cmDocumentationEntry cmDocumentationCommandsHeader[] =
{
  {0,
   "The following commands are available in CMakeLists.txt code:", 0},
  {0,0,0}
};

//----------------------------------------------------------------------------
static const cmDocumentationEntry cmDocumentationGeneratorsHeader[] =
{
  {0,
   "The following generators are available on this platform:", 0},
  {0,0,0}
};

//----------------------------------------------------------------------------
const cmDocumentationEntry cmDocumentationMailingList[] =
{
  {0,
   "For help and discussion about using cmake, a mailing list is provided "
   "at cmake@www.cmake.org.  Please first read the full documentation at "
   "http://www.cmake.org before posting questions to the list.", 0},
  {0,0,0}
};

//----------------------------------------------------------------------------
const cmDocumentationEntry cmDocumentationAuthor[] =
{
  {0,
   "This manual page was generated by the \"--help-man\" option.", 0},
  {0,0,0}
};

//----------------------------------------------------------------------------
const cmDocumentationEntry cmDocumentationCopyright[] =
{
  {0,
   "Copyright (c) 2002 Kitware, Inc., Insight Consortium.  "
   "All rights reserved.", 0},
  {0,
   "Redistribution and use in source and binary forms, with or without "
   "modification, are permitted provided that the following conditions are "
   "met:", 0},
  {"",
   "Redistributions of source code must retain the above copyright notice, "
   "this list of conditions and the following disclaimer.", 0},
  {"",
   "Redistributions in binary form must reproduce the above copyright "
   "notice, this list of conditions and the following disclaimer in the "
   "documentation and/or other materials provided with the distribution.",
   0},
  {"",
   "The names of Kitware, Inc., the Insight Consortium, or the names of "
   "any consortium members, or of any contributors, may not be used to "
   "endorse or promote products derived from this software without "
   "specific prior written permission.", 0},
  {"",
   "Modified source versions must be plainly marked as such, and must "
   "not be misrepresented as being the original software.", 0},
  {0,
   "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "
   "``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT "
   "LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR "
   "A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR "
   "CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, "
   "EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, "
   "PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR "
   "PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF "
   "LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING "
   "NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS "
   "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.", 0},
  {0, 0, 0}
};

//----------------------------------------------------------------------------
cmDocumentation::cmDocumentation()
{
  this->CurrentForm = TextForm;
  this->TextIndent = "";
  this->TextWidth = 77;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintCopyright(std::ostream& os)
{
  os << "CMake version " CMake_VERSION_FULL "\n";
  for(const cmDocumentationEntry* op = cmDocumentationCopyright;
      op->brief; ++op)
    {
    if(op->name)
      {
      os << " * ";
      this->TextIndent = "    ";
      this->PrintColumn(os, op->brief);
      }
    else
      {
      this->TextIndent = "";
      this->PrintColumn(os, op->brief);
      }
    os << "\n";
    }
  return true;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintVersion(std::ostream& os)
{
  os << this->GetNameString() << " version " CMake_VERSION_FULL "\n";
  return true;
}

//----------------------------------------------------------------------------
void cmDocumentation::AddSection(const char* name,
                                 const cmDocumentationEntry* d)
{
  this->Names.push_back(name);
  this->Sections.push_back(d);
}

//----------------------------------------------------------------------------
void cmDocumentation::ClearSections()
{
  this->Names.erase(this->Names.begin(), this->Names.end());
  this->Sections.erase(this->Sections.begin(), this->Sections.end());
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentation(Type ht, std::ostream& os)
{
  switch (ht)
    {
    case cmDocumentation::Usage:     return this->PrintDocumentationUsage(os);
    case cmDocumentation::Single:    return this->PrintDocumentationSingle(os);
    case cmDocumentation::List:      return this->PrintDocumentationList(os);
    case cmDocumentation::Full:      return this->PrintDocumentationFull(os);
    case cmDocumentation::HTML:      return this->PrintDocumentationHTML(os);
    case cmDocumentation::Man:       return this->PrintDocumentationMan(os);
    case cmDocumentation::Copyright: return this->PrintCopyright(os);
    case cmDocumentation::Version:   return this->PrintVersion(os);
    default: return false;
    }
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintRequestedDocumentation(std::ostream& os)
{
  bool result = true;
  
  // Loop over requested documentation types.
  for(RequestedMapType::const_iterator i = this->RequestedMap.begin();
      i != this->RequestedMap.end(); ++i)
    {
    // Special case for printing help for a single command.
    if(i->first == cmDocumentation::Usage && i->second.length() > 0 &&
       !this->CommandsSection.empty())
      {
      // Check if the argument to the usage request was a command.
      for(cmDocumentationEntry* entry = &this->CommandsSection[0];
          entry->brief; ++entry)
        {
        if(entry->name && (strcmp(entry->name, i->second.c_str()) == 0))
          {
          this->PrintDocumentationCommand(os, entry);
          return true;
          }
        }
      
      // Argument was not a command.  Complain.
      os << "Help argument \"" << i->second.c_str()
         << "\" is not a CMake command.  "
         << "Use --help-command-list to see all commands.\n";
      return false;
      }
    
    // If a file name was given, use it.  Otherwise, default to the
    // given stream.
    std::ofstream* fout = 0;
    std::ostream* s = &os;
    if(i->second.length() > 0)
      {
      fout = new std::ofstream(i->second.c_str(), std::ios::out);
      if(fout)
        {
        s = fout;
        }
      else
        {
        result = false;
        }
      }
    
    // Print this documentation type to the stream.
    if(!this->PrintDocumentation(i->first, *s) || !*s)
      {
      result = false;
      }
    
    // Close the file if we wrote one.
    if(fout)
      {
      delete fout;
      }
    }
  return result;
}

//----------------------------------------------------------------------------
bool cmDocumentation::CheckOptions(int argc, const char* const* argv)
{
  // Providing zero arguments gives usage information.
  if(argc == 1)
    {
    this->RequestedMap[cmDocumentation::Usage] = "";
    return true;
    }
  
  // Search for supported help options.
  bool result = false;
  for(int i=1; i < argc; ++i)
    {
    // Check if this is a supported help option.
    Type type = cmDocumentation::None;
    if((strcmp(argv[i], "-help") == 0) ||
       (strcmp(argv[i], "--help") == 0) ||
       (strcmp(argv[i], "/?") == 0) ||
       (strcmp(argv[i], "-usage") == 0) ||
       (strcmp(argv[i], "-h") == 0) ||
       (strcmp(argv[i], "-H") == 0))
      {
      type = cmDocumentation::Usage;
      }
    else if(strcmp(argv[i], "--help-full") == 0)
      {
      type = cmDocumentation::Full;
      }
    else if(strcmp(argv[i], "--help-html") == 0)
      {
      type = cmDocumentation::HTML;
      }
    else if(strcmp(argv[i], "--help-man") == 0)
      {
      type = cmDocumentation::Man;
      }
    else if(strcmp(argv[i], "--help-command") == 0)
      {
      type = cmDocumentation::Single;
      if((i+1 < argc) && !this->IsOption(argv[i+1]))
        {
        this->SingleCommand = argv[i+1];
        i = i+1;
        }
      }
    else if(strcmp(argv[i], "--help-command-list") == 0)
      {
      type = cmDocumentation::List;
      }
    else if(strcmp(argv[i], "--copyright") == 0)
      {
      type = cmDocumentation::Copyright;
      }
    else if((strcmp(argv[i], "--version") == 0) || 
            (strcmp(argv[i], "-version") == 0) || 
            (strcmp(argv[i], "/V") == 0))
      {
      type = cmDocumentation::Version;
      }
    if(type)
      {
      // This is a help option.  See if there is a file name given.
      result = true;
      if((i+1 < argc) && !this->IsOption(argv[i+1]))
        {
        this->RequestedMap[type] = argv[i+1];
        i = i+1;
        }
      else
        {
        this->RequestedMap[type] = "";
        }
      }
    }
  return result;
}

//----------------------------------------------------------------------------
void cmDocumentation::Print(Form f, std::ostream& os)
{
  this->CurrentForm = f;
  for(unsigned int i=0; i < this->Sections.size(); ++i)
    {
    this->PrintSection(os, this->Sections[i], this->Names[i]);
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::SetName(const char* name)
{
  this->NameString = name?name:"";
}

//----------------------------------------------------------------------------
void cmDocumentation::SetNameSection(const cmDocumentationEntry* section)
{
  this->SetSection(0, section, 0, this->NameSection);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetUsageSection(const cmDocumentationEntry* section)
{
  this->SetSection(0, section, 0, this->UsageSection);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetDescriptionSection(const cmDocumentationEntry* section)
{
  this->SetSection(0, section, 0, this->DescriptionSection);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetOptionsSection(const cmDocumentationEntry* section)
{
  this->SetSection(0, section, cmDocumentationStandardOptions,
                   this->OptionsSection);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetCommandsSection(const cmDocumentationEntry* section)
{
  this->SetSection(cmDocumentationCommandsHeader, section, 0,
                   this->CommandsSection);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetGeneratorsSection(const cmDocumentationEntry* section)
{
  this->SetSection(cmDocumentationGeneratorsHeader, section, 0,
                   this->GeneratorsSection);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetSeeAlsoList(const cmDocumentationEntry* also)
{
  this->SeeAlsoString = ".B ";
  for(const cmDocumentationEntry* i = also; i->brief; ++i)
    {
    this->SeeAlsoString += i->brief;
    this->SeeAlsoString += (i+1)->brief? "(1), ":"(1)";    
    }
  cmDocumentationEntry e = {0, 0, 0};
  e.brief = this->SeeAlsoString.c_str();
  this->SeeAlsoSection.push_back(e);
  e.brief = 0;
  this->SeeAlsoSection.push_back(e);  
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintSection(std::ostream& os,
                                   const cmDocumentationEntry* section,
                                   const char* name)
{
  switch (this->CurrentForm)
    {
    case TextForm: this->PrintSectionText(os, section, name); break;
    case HTMLForm: this->PrintSectionHTML(os, section, name); break;
    case ManForm: this->PrintSectionMan(os, section, name); break;
    case UsageForm: this->PrintSectionUsage(os, section, name); break;
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintSectionText(std::ostream& os,
                                       const cmDocumentationEntry* section,
                                       const char* name)
{
  if(name)
    {
    os <<
      "---------------------------------------"
      "---------------------------------------\n";
    os << name << "\n\n";
    }
  if(!section) { return; }
  for(const cmDocumentationEntry* op = section; op->brief; ++op)
    {
    if(op->name)
      {
      if(op->name[0])
        {
        os << "  " << op->name << "\n";
        }
      this->TextIndent = "       ";
      this->PrintFormatted(os, op->brief);
      if(op->full)
        {
        os << "\n";
        this->PrintFormatted(os, op->full);
        }
      }
    else
      {
      this->TextIndent = "";
      this->PrintFormatted(os, op->brief);
      }
    os << "\n";
    }  
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintSectionHTML(std::ostream& os,
                                       const cmDocumentationEntry* section,
                                       const char* name)
{
  if(name)
    {
    os << "<h2>" << name << "</h2>\n";
    }
  if(!section) { return; }
  for(const cmDocumentationEntry* op = section; op->brief;)
    {
    if(op->name)
      {
      os << "<ul>\n";
      for(;op->name;++op)
        {
        os << "  <li>\n";
        if(op->name[0])
          {
          os << "    <b><code>";
          this->PrintHTMLEscapes(os, op->name);
          os << "</code></b>: ";
          }
        this->PrintHTMLEscapes(os, op->brief);
        if(op->full)
          {
          os << "<br>\n    ";
          this->PrintFormatted(os, op->full);
          }
        os << "\n";
        os << "  </li>\n";
        }
      os << "</ul>\n";
      }
    else
      {
      this->PrintFormatted(os, op->brief);
      os << "\n";
      ++op;
      }
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintSectionMan(std::ostream& os,
                                      const cmDocumentationEntry* section,
                                      const char* name)
{
  if(name)
    {
    os << ".SH " << name << "\n";
    }
  if(!section) { return; }
  for(const cmDocumentationEntry* op = section; op->brief; ++op)
    {
    if(op->name)
      {
      os << ".TP\n"
         << ".B " << (op->name[0]?op->name:"*") << "\n";
      this->PrintFormatted(os, op->brief);
      this->PrintFormatted(os, op->full);
      }
    else
      {
      os << ".PP\n";
      this->PrintFormatted(os, op->brief);
      }
    }  
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintSectionUsage(std::ostream& os,
                                        const cmDocumentationEntry* section,
                                        const char* name)
{
  if(name)
    {
    os << name << "\n";
    }
  if(!section) { return; }
  for(const cmDocumentationEntry* op = section; op->brief; ++op)
    {
    if(op->name)
      {
      os << "  " << op->name;
      this->TextIndent = "                                ";
      int align = static_cast<int>(strlen(this->TextIndent))-4;
      for(int i = static_cast<int>(strlen(op->name)); i < align; ++i)
        {
        os << " ";
        }
      os << "= ";
      this->PrintColumn(os, op->brief);
      os << "\n";
      }
    else
      {
      os << "\n";
      this->TextIndent = "";
      this->PrintFormatted(os, op->brief);
      }
    }
  os << "\n";
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintFormatted(std::ostream& os, const char* text)
{
  if(!text)
    {
    return;
    }
  const char* ptr = text;
  while(*ptr)
    {
    // Any ptrs starting in a space are treated as preformatted text.
    std::string preformatted;
    while(*ptr == ' ')
      {
      for(char ch = *ptr; ch && ch != '\n'; ++ptr, ch = *ptr)
        {
        preformatted.append(1, ch);
        }
      if(*ptr)
        {
        ++ptr;
        preformatted.append(1, '\n');
        }
      }
    if(preformatted.length())
      {
      this->PrintPreformatted(os, preformatted.c_str());
      }
    
    // Other ptrs are treated as paragraphs.
    std::string paragraph;
    for(char ch = *ptr; ch && ch != '\n'; ++ptr, ch = *ptr)
      {
      paragraph.append(1, ch);
      }
    if(*ptr)
      {
      ++ptr;
      paragraph.append(1, '\n');
      }
    if(paragraph.length())
      {
      this->PrintParagraph(os, paragraph.c_str());
      }
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintPreformatted(std::ostream& os, const char* text)
{
  switch (this->CurrentForm)
    {
    case TextForm: this->PrintPreformattedText(os, text); break;
    case HTMLForm: this->PrintPreformattedHTML(os, text); break;
    case ManForm: this->PrintPreformattedMan(os, text); break;
    case UsageForm: this->PrintPreformattedText(os, text); break;
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintParagraph(std::ostream& os, const char* text)
{
  switch (this->CurrentForm)
    {
    case TextForm: this->PrintParagraphText(os, text); break;
    case HTMLForm: this->PrintParagraphHTML(os, text); break;
    case ManForm: this->PrintParagraphMan(os, text); break;
    case UsageForm: this->PrintParagraphText(os, text); break;
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintPreformattedText(std::ostream& os, const char* text)
{
  bool newline = true;
  for(const char* ptr = text; *ptr; ++ptr)
    {
    if(newline)
      {
      os << this->TextIndent;
      newline = false;
      }
    os << *ptr;
    if(*ptr == '\n')
      {
      newline = true;
      }
    }
  os << "\n";
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintParagraphText(std::ostream& os, const char* text)
{
  os << this->TextIndent;
  this->PrintColumn(os, text);
  os << "\n";
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintPreformattedHTML(std::ostream& os, const char* text)
{
  os << "<pre>";
  this->PrintHTMLEscapes(os, text);
  os << "</pre>\n    ";
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintParagraphHTML(std::ostream& os, const char* text)
{
  os << "<p>";
  this->PrintHTMLEscapes(os, text);
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintPreformattedMan(std::ostream& os, const char* text)
{
  os << text << "\n";
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintParagraphMan(std::ostream& os, const char* text)
{
  os << text << "\n\n";
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintColumn(std::ostream& os, const char* text)
{
  // Print text arranged in an indented column of fixed witdh.
  const char* l = text;
  int column = 0;
  bool newSentence = false;
  bool firstLine = true;
  int width = this->TextWidth - static_cast<int>(strlen(this->TextIndent));
  
  // Loop until the end of the text.
  while(*l)
    {
    // Parse the next word.
    const char* r = l;
    while(*r && (*r != '\n') && (*r != ' ')) { ++r; }
    
    // Does it fit on this line?
    if(r-l < (width-column-(newSentence?1:0)))
      {
      // Word fits on this line.
      if(r > l)
        {
        if(column)
          {
          // Not first word on line.  Separate from the previous word
          // by a space, or two if this is a new sentence.
          if(newSentence)
            {
            os << "  ";
            column += 2;
            }
          else
            {
            os << " ";
            column += 1;
            }
          }
        else
          {
          // First word on line.  Print indentation unless this is the
          // first line.
          os << (firstLine?"":this->TextIndent);
          }
        
        // Print the word.
        os.write(l, static_cast<long>(r-l));
        newSentence = (*(r-1) == '.');
        }
      
      if(*r == '\n')
        {
        // Text provided a newline.  Start a new line.
        os << "\n";
        ++r;
        column = 0;
        firstLine = false;
        }
      else
        {
        // No provided newline.  Continue this line.
        column += static_cast<long>(r-l);
        }
      }
    else
      {
      // Word does not fit on this line.  Start a new line.
      os << "\n";
      firstLine = false;
      if(r > l)
        {
        os << this->TextIndent;
        os.write(l, static_cast<long>(r-l));
        column = static_cast<long>(r-l);
        newSentence = (*(r-1) == '.');
        }
      }
    
    // Move to beginning of next word.  Skip over whitespace.
    l = r;
    while(*l && (*l == ' ')) { ++l; }    
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintHTMLEscapes(std::ostream& os, const char* text)
{
  static cmDocumentationEntry escapes[] =
  {
    {"<", "&lt;", 0},
    {">", "&gt;", 0},
    {"&", "&amp;", 0},
    {"\n", "<br>", 0},
    {0,0,0}
  };
  for(const char* p = text; *p; ++p)
    {
    bool found = false;
    for(const cmDocumentationEntry* op = escapes; !found && op->name; ++op)
      {
      if(op->name[0] == *p)
        {
        os << op->brief;
        found = true;
        }
      }
    if(!found)
      {
      os << *p;
      }
    }
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentationSingle(std::ostream& os)
{
  if(this->CommandsSection.empty())
    {
    os << "Internal error: commands list is empty." << std::endl;
    return false;
    }
  if(this->SingleCommand.length() == 0)
    {
    os << "Argument --help-command needs a command name.\n";
    return false;
    }
  for(cmDocumentationEntry* entry = &this->CommandsSection[0];
      entry->brief; ++entry)
    {
    if(entry->name && this->SingleCommand == entry->name)
      {
      this->PrintDocumentationCommand(os, entry);
      return true;
      }
    }
  // Argument was not a command.  Complain.
  os << "Argument \"" << this->SingleCommand.c_str()
     << "\" to --help-command is not a CMake command.  "
     << "Use --help-command-list to see all commands.\n";
  return false;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentationList(std::ostream& os)
{
  if(this->CommandsSection.empty())
    {
    os << "Internal error: commands list is empty." << std::endl;
    return false;
    }
  for(cmDocumentationEntry* entry = &this->CommandsSection[0];
      entry->brief; ++entry)
    {
    if(entry->name)
      {
      os << entry->name << std::endl;
      }
    }
  return true;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentationUsage(std::ostream& os)
{
  this->CreateUsageDocumentation();
  this->Print(UsageForm, os);
  return true;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentationFull(std::ostream& os)
{
  this->CreateFullDocumentation();
  this->Print(TextForm, os);
  return true;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentationHTML(std::ostream& os)
{
  this->CreateFullDocumentation();
  os << "<html><body>\n";
  this->Print(HTMLForm, os);
  os << "</body></html>\n";
  return true;
}

//----------------------------------------------------------------------------
bool cmDocumentation::PrintDocumentationMan(std::ostream& os)
{
  this->CreateManDocumentation();
  os << ".TH " << this->GetNameString() << " 1 \""
     << cmSystemTools::GetCurrentDateTime("%B %d, %Y").c_str()
     << "\" \"" << this->GetNameString() << " " CMake_VERSION_FULL "\"\n";
  this->Print(ManForm, os);
  return true;
}

//----------------------------------------------------------------------------
void cmDocumentation::PrintDocumentationCommand(std::ostream& os,
                                                cmDocumentationEntry* entry)
{
  cmDocumentationEntry singleCommandSection[3] =
    {
      {entry->name, entry->brief, entry->full},
      {0,0,0}
    };
  this->ClearSections();
  this->AddSection(0, &singleCommandSection[0]);
  this->Print(TextForm, os);
}

//----------------------------------------------------------------------------
void cmDocumentation::CreateUsageDocumentation()
{
  this->ClearSections();
  if(!this->UsageSection.empty())
    {
    this->AddSection("Usage", &this->UsageSection[0]);
    }
  if(!this->OptionsSection.empty())
    {
    this->AddSection("Command-Line Options", &this->OptionsSection[0]);
    }
  if(!this->GeneratorsSection.empty())
    {
    this->AddSection("Generators", &this->GeneratorsSection[0]);
    }
}

//----------------------------------------------------------------------------
void cmDocumentation::CreateFullDocumentation()
{
  this->ClearSections();
  if(!this->NameSection.empty())
    {
    this->AddSection("Name", &this->NameSection[0]);
    }
  if(!this->UsageSection.empty())
    {
    this->AddSection("Usage", &this->UsageSection[0]);
    }
  if(!this->DescriptionSection.empty())
    {
    this->AddSection(0, &this->DescriptionSection[0]);
    }
  if(!this->OptionsSection.empty())
    {
    this->AddSection("Command-Line Options", &this->OptionsSection[0]);
    }
  if(!this->GeneratorsSection.empty())
    {
    this->AddSection("Generators", &this->GeneratorsSection[0]);
    }
  if(!this->CommandsSection.empty())
    {
    this->AddSection("Listfile Commands", &this->CommandsSection[0]);
    }
  this->AddSection("Copyright", cmDocumentationCopyright);
  this->AddSection("Mailing List", cmDocumentationMailingList);
}

//----------------------------------------------------------------------------
void cmDocumentation::CreateManDocumentation()
{
  this->ClearSections();
  if(!this->NameSection.empty())
    {
    this->AddSection("NAME", &this->NameSection[0]);
    }
  if(!this->UsageSection.empty())
    {
    this->AddSection("SYNOPSIS", &this->UsageSection[0]);
    }
  if(!this->DescriptionSection.empty())
    {
    this->AddSection("DESCRIPTION", &this->DescriptionSection[0]);
    }
  if(!this->OptionsSection.empty())
    {
    this->AddSection("OPTIONS", &this->OptionsSection[0]);
    }
  if(!this->GeneratorsSection.empty())
    {
    this->AddSection("GENERATORS", &this->GeneratorsSection[0]);
    }
  if(!this->CommandsSection.empty())
    {
    this->AddSection("COMMANDS", &this->CommandsSection[0]);
    }
  this->AddSection("COPYRIGHT", cmDocumentationCopyright);
  this->AddSection("MAILING LIST", cmDocumentationMailingList);  
  if(!this->SeeAlsoSection.empty())
    {
    this->AddSection("SEE ALSO", &this->SeeAlsoSection[0]);
    }
  this->AddSection("AUTHOR", cmDocumentationAuthor);
}

//----------------------------------------------------------------------------
void cmDocumentation::SetSection(const cmDocumentationEntry* header,
                                 const cmDocumentationEntry* section,
                                 const cmDocumentationEntry* footer,
                                 std::vector<cmDocumentationEntry>& vec)
{
  vec.erase(vec.begin(), vec.end());
  if(header)
    {
    for(const cmDocumentationEntry* op = header; op->brief; ++op)
      {
      vec.push_back(*op);
      }
    }
  if(section)
    {
    for(const cmDocumentationEntry* op = section; op->brief; ++op)
      {
      vec.push_back(*op);
      }
    }
  if(footer)
    {
    for(const cmDocumentationEntry* op = footer; op->brief; ++op)
      {
      vec.push_back(*op);
      }
    }
  cmDocumentationEntry empty = {0,0,0};
  vec.push_back(empty);  
}

//----------------------------------------------------------------------------
const char* cmDocumentation::GetNameString()
{
  if(this->NameString.length() > 0)
    {
    return this->NameString.c_str();
    }
  else
    {
    return "CMake";
    }
}

//----------------------------------------------------------------------------
bool cmDocumentation::IsOption(const char* arg)
{
  return ((arg[0] == '-') ||
          (strcmp(arg, "/V") == 0) ||
          (strcmp(arg, "/?") == 0));
}
