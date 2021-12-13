# CleanUpSamplesNames
## A program designed to cleanup the filenames of sample libraries. 
  
Usage: CleanUpSamplesNames --input-dir=myInputDir --output-dir=myOutputDir --prefix=myNewPrefix  
  
input directory is the directory containing awkwardly named samples.  
  
output directory is the directory you want to copy the samples into with the new names.  

prefix is the prefix for the new files.  
  
for example:  
* if the old samples are called  
  * lkajwr09F987nqy9_G#5.wav  
  * lkajwr09F987nqy9_A5.wav  
  * lkajwr09F987nqy9_A#5.wav  
  * lkajwr09F987nqy9_B5.wav  
  * etc.....  
* and your prefix is "AwesomeSamples_"
* your new samples will be called  
  * AwesomeSamples_G#5.wav  
  * AwesomeSamples_A5.wav  
  * AwesomeSamples_A#5.wav  
  * AwesomeSamples_B5.wav  
  * etc.....  
