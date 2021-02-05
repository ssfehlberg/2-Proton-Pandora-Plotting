#include <sys/types.h>
#include <sys/stat.h>

const char* which_sample(){

  char response;
  const char* sample;
  std::cout<<"Which Sample is This?"<<std::endl;
  std::cout<<" 0 = pelee \n 1 = filtered \n 2 = unfiltered"<<std::endl;
  std::cin>>response;

  if(response =='0'){
    sample = "pelee";
  }else if (response =='1'){
    sample = "unfiltered";
  }else if(response == '2'){
    sample = "unfiltered";
  }else{
      std::cout<<"Invalid Response. Please Type 0, 1, or 2 for pelee, filtered, and unfiltered samples respectively."<<std::endl;
  }

  return sample;
}

/******************************************************************************                                                                                                                                                      
 * Checks to see if a directory exists. Note: This method only checks the                                                                                                                                                            
 * existence of the full path AND if path leaf is a dir.                                                                                                                                                                             
 *                                                                                                                                                                                                                                   
 * @return  >0 if dir exists AND is a dir,                                                                                                                                                                                           
 *           0 if dir does not exist OR exists but not a dir,                                                                                                                                                                        
 *          <0 if an error occurred (errno is also set)                                                                                                                                                                              
 *****************************************************************************/

int dirExists(const char* const path)
{
    struct stat info;

    int statRC = stat( path, &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT)  { return 0; } // something along the path does not exist                                                                                                                                                     
        if (errno == ENOTDIR) { return 0; } // something in path prefix is not a dir                                                                                                                                                       
        return -1;
    }

    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
}

double calculatePearsonChiSq(TH1D* O, TH1D* E){
    double chisq = 0;
    for (int i = 1; i < O->GetNbinsX()+1; i++){
        double O_i = O->GetBinContent(i);
        double E_i = E->GetBinContent(i);
        if (O_i == 0 && E_i == 0){
            chisq += 0;
        }

        else{
            chisq += std::pow(O_i - E_i,2)/((O_i+E_i)/2);
        }
    }
    return chisq;
}

void OverlayStatistics(TH1D* O, TH1D* E, TH1D* L){
  for(int i_bin = 1; i_bin < O->GetXaxis()->GetNbins(); i_bin++){
    double total_mc_in_bin = O->GetBinContent(i_bin);
    double total_ext_in_bin = E->GetBinContent(i_bin);
    double total_dirt_in_bin = L->GetBinContent(i_bin);
    O->SetBinContent(i_bin,(total_mc_in_bin+total_ext_in_bin+total_dirt_in_bin));
    O->SetBinError(i_bin, std::sqrt(total_mc_in_bin + total_ext_in_bin + total_dirt_in_bin));
  }
}
