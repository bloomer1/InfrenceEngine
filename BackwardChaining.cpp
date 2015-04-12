/*
 * BackwardChaining.cpp
 *
 *  Created on: 12-Nov-2014
 *      Author: rahulagarwal
 */

#include<iostream>
#include<cstring>
#include<cstdio>
#include<fstream>
#include<map>
#include<vector>

using namespace std;


FILE* ifp;
FILE* ofp;

char querystring[500];
int noOfClauses;

void predicateParser(char []);
void premiseParser(string);
bool backwardChaining(string);
bool premise_checker(string,vector<string>);


map<string,vector<string> > conclusion_premise_map;
map<string, vector<string> >predicate_literal_map;

vector<string> conclusion_vector;
vector<string> premise_vector;
vector<string> clause_vector;


int main(){

    bool result = false;;
    ifp = fopen("/Users/rahulagarwal/Desktop/Fall2014/CSCI561/RAHUL_AGARWAL_HW3/input.txt","r");
    ofp = fopen("/Users/rahulagarwal/Desktop/Fall2014/CSCI561/RAHUL_AGARWAL_HW3/output.txt","w");

    fscanf(ifp,"%s",&querystring);
    fscanf(ifp,"%d",&noOfClauses);

    char clause[500];
    for(int i = 0 ; i < noOfClauses;i++){
        fscanf(ifp,"%s",&clause);
        clause_vector.push_back(clause);
        predicateParser(clause);
    }

    map<string,map<string,string> >::iterator it;
    map<string,string>::iterator it2;



    // cout<<"conclusion list"<<endl;
    vector<string>::iterator vt;
    //    for(vt = conclusion_vector.begin(); vt != conclusion_vector.end(); vt++){
    //          cout<<*vt<<" ";
    //    }
    //      cout<<endl;

    for(vt = clause_vector.begin(); vt!= clause_vector.end();vt++){
        premiseParser(*vt);
    }
   // cout<<"The mappings conclusion-premise"<<endl;
    map<string,vector<string> >::iterator conpre;
//    for(conpre = conclusion_premise_map.begin(); conpre!= conclusion_premise_map.end(); conpre++){
//        cout<<conpre->first<<"->";
//        for(vt = conpre->second.begin(); vt != conpre->second.end() ;vt++){
//            cout<<*vt<<" ";
//        }
//        cout<<endl;
//    }
   // cout<<endl;
    //    cout<<"The mappings predicate literal "<<endl;
    //    for(conpre = predicate_literal_map.begin(); conpre!= predicate_literal_map.end();conpre++){
    //    	cout<<conpre->first<<"->";
    //    	for(vt = conpre->second.begin(); vt != conpre->second.end() ;vt++){
    //    		cout<<*vt<<" ";
    //    	}
    //    	cout<<endl;
    //    }

    result = backwardChaining(querystring);
    if(result){
        //cout<<"TRUE"<<endl;
        fprintf(ofp,"TRUE");
        fprintf(ofp,"\n");

    }
    else{
        //cout<<"FALSE"<<endl;
        fprintf(ofp,"FALSE");
        fprintf(ofp,"\n");


    }

   // cout<<"result "<<result;


    return 0;
}

void predicateParser(char clause[]){

    string token;
    string innertoken;
    string innermosttoken;
    string s = clause;
    vector<string> literal;
    bool multi_implication = false;
    bool contains_implication = false;
    bool valid_conclusion_token = false;
    size_t pos = 0;
    size_t innerpos = 0;
    size_t litpos = 0;

    while(pos != string::npos){
        if((pos = s.find("=>")) != string::npos){
            token = s.substr(0, pos);
            if(multi_implication){
                conclusion_vector.push_back(token);
                valid_conclusion_token = true;
            }
            multi_implication = true;
            contains_implication = true;
        }
        else{
            token = s;
            if(contains_implication){
                conclusion_vector.push_back(token);
                valid_conclusion_token = true;
            }

        }

        while((innerpos != string::npos)){

            if((innerpos = token.find("&")) != string::npos){
                innertoken = token.substr(0,innerpos);
            }
            else{
                innertoken = token;
            }
            litpos = innertoken.find("(");
            char lit = innertoken[litpos + 1];
            //here also valid literal
            if(lit != 'x')
                literal.push_back(innertoken);
            if((litpos = innertoken.find(",",litpos)) != string::npos){
                char lit2 = innertoken[litpos + 1];
                if(lit != 'x' && lit2 !='x'){
                    literal.push_back(innertoken);
                }
            }
            innermosttoken = innertoken.substr(0,token.find("("));

            if(innermosttoken != ""){
                predicate_literal_map[innermosttoken] = literal;
            }
            if(innerpos != string::npos)
                token.erase(0,innerpos + 1);
            else
                token.clear();
        }
        if(pos != string::npos)
            s.erase(0, pos + 2);
        else
            s.clear();

    }
}


void premiseParser(string clause){

    size_t pos = 0;
    size_t innerpos = 0;
    string sclause = clause;
    string subclause;
    string premise;
    vector<string> premisevect;
    vector<string>::iterator vect;
    vector<string>::iterator vect1;

    for(vect = conclusion_vector.begin(); vect != conclusion_vector.end(); vect++){
        while(pos != string::npos){
            if((pos = sclause.find("=>")) != string::npos){
                subclause = sclause.substr(0,pos);
                if(sclause.find(*vect,pos+2) != string::npos){
                    while(innerpos != string::npos){
                        if((innerpos = subclause.find("&")) != string::npos){
                            premise = subclause.substr(0,innerpos);
                        }
                        else{
                            premise = subclause;
                        }
                        premisevect.push_back(premise);
                        if(innerpos != string::npos)
                            subclause.erase(0,innerpos + 1);
                        else
                            subclause.clear();
                    }
                    if(conclusion_premise_map.find(*vect) == conclusion_premise_map.end())
                        conclusion_premise_map[*vect] = premisevect;
                }

            }
            if(pos != string::npos)
                sclause.erase(0,pos +2);
            else
                sclause.clear();
        }
        premisevect.clear();
        pos = 0;
        innerpos  = 0;
        sclause = clause;//the master bug...

    }
}

bool backwardChaining(string query){

    bool result = false;
    bool found = false;
    string query_preserve = query;
    string first_literal ="";
    string second_literal = "";
    string query_token;
    string conclusion;
    string conclusion_copy;
    size_t pos = 0;
    size_t innerpos = 0;

    vector<string> literal_vect;
    map<string,vector<string> >::iterator con_it;
    vector<string>::iterator literal_vect_it;

    //cout<<"in backward chaining "<<query<<endl;





    //variable matchings
    pos = query.find("(");
    query_token = query.substr(pos+1);
    innerpos = query_token.find(",");
    if(innerpos != string::npos){
        first_literal = query_token.substr(0,innerpos);
        //cout<<"first literal "<<first_literal<<endl;
        query_token = query_token.substr(innerpos+1);
        innerpos = query_token.find(")");
        second_literal = query_token.substr(0,innerpos);
        //cout<<"Second Literal "<<second_literal<<endl;
    }
    else{
        innerpos = query_token.find(")");
        first_literal = query_token.substr(0,innerpos);
       // cout<<"only first "<<first_literal<<endl;
    }
   // cout<<"query = "<<query<<" "<<"first_literal = "<<first_literal<<" "<<"second_literal "<<second_literal<<endl;
    if(first_literal != "x" && second_literal != "x"){
        if(second_literal != ""){
            for(con_it = conclusion_premise_map.begin(); con_it != conclusion_premise_map.end();con_it++){

                conclusion = con_it->first;
                conclusion_copy = conclusion;

                pos = conclusion.find("(");
                conclusion = conclusion.substr(pos+1);
                innerpos = conclusion.find(",");
                if(innerpos != string::npos){
                    if((conclusion.substr(0,innerpos)) == "x"){
                        conclusion = conclusion_copy;
                        conclusion.replace(pos+1,1,first_literal);
                        //cout<<"conclu "<<conclusion<<" "<<pos<<endl;
                        if(conclusion == query){
                            //int size = conclusion_premise_map[con_it->first].size();
                            //bool resultarray[size];



                            result = premise_checker(first_literal,conclusion_premise_map[con_it->first]);
                            found = true;
                            //something to return from here...




                        }
                    }
                    else if(conclusion.substr(innerpos + 1,1) == "x"){
                        conclusion = conclusion_copy;
                        conclusion.replace(innerpos+1,1,second_literal);
                        if(conclusion == query){
                            result = premise_checker(second_literal,conclusion_premise_map[con_it->first]);
                            found = true;
                        }

                    }
                    else if(conclusion_copy == query){
                        //both literals are not x in the conclusion...
                        result = premise_checker("",conclusion_premise_map[con_it->first]);
                        found = true;

                    }

                }






            }

            if(!found){
                //literal checking...
                //cout<<" inside literal checking "<<query<<endl;
                pos = query.find("(");
                query_token = query.substr(0,pos);
                //cout<<"inside literal-2 checking "<<query_token<<endl;
                literal_vect = predicate_literal_map[query_token];
        if(!literal_vect.empty()){
        for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                        if(*literal_vect_it == query){
                            //cout<<"yes"<<endl;
                            return true;
                        }
                        else
                            return false;
                    }

                }

            }

        }
        else{

            //cout<<"in in"<<endl;
            //processing only if query has one literal...
            for(con_it = conclusion_premise_map.begin();con_it != conclusion_premise_map.end();con_it++){
                conclusion = con_it->first;
                conclusion_copy = conclusion;
                // cout<<" in in "<<conclusion<<endl;
                pos = conclusion.find("(");
                conclusion = conclusion.substr(pos+1);
                //cout<<"in in "<<conclusion<<endl;
                innerpos = conclusion.find(",");


                if(conclusion.substr(0,1) == "x"){
                    conclusion = conclusion_copy;
                    conclusion.replace(pos+1,1,first_literal);
                    //cout<<"inside inside "<<conclusion<<" "<<query<<endl;
                    if(conclusion == query){
                        result = premise_checker(first_literal,conclusion_premise_map[con_it->first]);
                    }
                }
                else if(conclusion_copy == query) {
                    //literal is not x...
                    result = premise_checker("",conclusion_premise_map[con_it->first]);
                   // cout<<"res finally 2"<<result<<endl;

                }
                // }


            }

            //here literal checking....

            pos = query.find("(");
            query_token = query.substr(0,pos);
            //cout<<"inside literal-2 checking "<<query_token<<endl;
            literal_vect = predicate_literal_map[query_token];
            if(!literal_vect.empty()){
                for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                    if(*literal_vect_it == query){
                       // cout<<"yes"<<endl;
                        return true;
                    }
                    else
                        return false;
                }

            }



        }


    }
    else{
        //first or second literal is x...
        bool found = false;
        if(first_literal == "x"){
           // cout<<"ok..."<<endl;
            for(con_it = conclusion_premise_map.begin();con_it != conclusion_premise_map.end();con_it++){
                conclusion = con_it->first;
                conclusion_copy = conclusion;

                pos = conclusion.find("(");
                conclusion = conclusion.substr(pos+1);
                innerpos = conclusion.find(",");
                //cout<<"pos "<<pos<<" innerpos "<<innerpos<<endl;
               // cout<<"que "<<query<<" "<<conclusion<<endl;
                if(innerpos != string::npos){
                    first_literal = conclusion.substr(0,innerpos);

                }
                else{
                    innerpos = conclusion.find(")");
                    first_literal = conclusion.substr(0,innerpos);
                }

                pos = query.find("(");
                query.replace(pos+1,1,first_literal);
                conclusion = conclusion_copy;
                if(conclusion == query){
                    //cout<<"yes pa"<<endl;
                    result = premise_checker("",conclusion_premise_map[con_it->first]);
                   // cout<<"yes res finally "<<endl;
                    found = true;
                }


            }

            //check for literal again...
            // cout<<"coming here..."<<query<<endl;
            if(!found){
                string second;
                query = query_preserve;
                //cout<<"coming here..."<<query<<endl;
                bool lit = true;//true
                pos = query.find("(");
                query_token = query.substr(0,pos);
                innerpos = query.find(",");
                literal_vect = predicate_literal_map[query_token];

                if(innerpos != string::npos){
                    pos = query.find(",");
                    innerpos = query.find(")");
                    second = query.substr(pos+1,innerpos);
                    //cout<<"sec..."<<second<<endl;
                    if(!literal_vect.empty()){
                        for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                            query = *literal_vect_it;
                            pos = query.find(",");
                            if(pos != string::npos){
                                innerpos = query.find(")");
                                query_token = query.substr(pos+1,innerpos);
                                //cout<<"after sec..."<<query_token<<endl;
                                if(query_token == second)
                                    lit = lit & true;
                                else{
                                    lit = lit & false;
                                }
                            }
                            else{
                                lit = lit & false;
                                //cout<<"lit work..."<<endl;

                            }
                        }
                    }
                    else{
                        lit = false;
                    }
                	   result = lit;

                }


                else{
                    //only one to match...

                    if(!literal_vect.empty()){

                        for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                            query =  *literal_vect_it;
                            pos = query.find(",");
                            if(pos == string::npos){
                                lit =  lit & true;
                            }
                            else{
                                lit = lit & false;
                            }

                        }

                    }
                    else{
                        lit = false;
                    }
                    result = lit;

                }
            }
        }







        else if(second_literal == "x"){


            for(con_it = conclusion_premise_map.begin();con_it != conclusion_premise_map.end();con_it++){
                conclusion = con_it->first;
                conclusion_copy = conclusion;

               // pos = conclusion.find("(");
                //conclusion = conclusion.substr(pos+1);
                pos = conclusion.find(",");


                //cout<<"pos "<<pos<<" innerpos "<<innerpos<<endl;
                //cout<<"que "<<query<<" "<<conclusion<<endl;


                if(pos != string::npos){
                    conclusion = conclusion.substr(pos+1);
                    innerpos = conclusion.find(")");
                    second_literal = conclusion.substr(0,innerpos);

                }


                pos = query.find("(");
                query.replace(pos+1,1,second_literal);
                conclusion = conclusion_copy;
                if(conclusion == query){
                    //cout<<"yes pa"<<endl;
                    result = premise_checker("",conclusion_premise_map[con_it->first]);
                    //cout<<"yes res finally "<<endl;
                    found = true;
                }


            }

            if(!found){
                string second;
                query = query_preserve;
                //cout<<"coming here..."<<query<<endl;
                bool lit = true;//true
                pos = query.find("(");
                query_token = query.substr(0,pos);
                innerpos = query.find(",");
                literal_vect = predicate_literal_map[query_token];

                if(innerpos != string::npos){
                    pos = query.find(",");
                    innerpos = query.find(")");
                    second = query.substr(pos+1,innerpos);
                   // cout<<"sec..."<<second<<endl;
                    if(!literal_vect.empty()){
                        for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                            query = *literal_vect_it;
                            pos = query.find(",");
                            if(pos != string::npos){
                                innerpos = query.find(")");
                                query_token = query.substr(pos+1,innerpos);
                               // cout<<"after sec..."<<query_token<<endl;
                                if(query_token == second)
                                    lit = lit & true;
                                else{
                                    lit = lit & false;
                                }
                            }
                            else{
                                lit = lit & false;
                                cout<<"lit work..."<<endl;

                            }
                        }
                    }
                    else{
                        lit = false;
                    }
                	   result = lit;

                }


                else{
                    //only one to match...

                    if(!literal_vect.empty()){

                        for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                            query =  *literal_vect_it;
                            pos = query.find(",");
                            if(pos == string::npos){
                                lit =  lit & true;
                            }
                            else{
                                lit = lit & false;
                            }

                        }

                    }
                    else{
                        lit = false;
                    }
                    result = lit;

                }
            }
        }







        }





















    //cout<<"in backward chain.."<<result<<endl;

    return result;
}

bool premise_checker(string literal, vector<string> dependency_premise){

    bool result = true;
    size_t pos;
    size_t innerpos;

    string premise;
    string pemise_token;
    vector<string> literal_vect;
    vector<string>::iterator literal_vect_it;

    vector<string>::iterator dependency_premise_it;




    if(literal != ""){
        for(dependency_premise_it = dependency_premise.begin();dependency_premise_it !=  dependency_premise.end();dependency_premise_it++){

            premise = *dependency_premise_it;
           // cout<<"before "<<premise<<endl;
            pos = premise.find("(");
            innerpos = premise.find(",");
            if(innerpos != string::npos){
                if(premise.substr(innerpos+1,1) == "x"){
                    premise.replace(innerpos+1,1,literal);
                    result = backwardChaining(premise) & result;
                }
                else{
                    premise.replace(pos+1,1,literal);
                    result =  backwardChaining(premise) & result;
                }
            }
            else{
                if(premise.substr(pos+1,1) == "x"){
                    premise.replace(pos+1,1,literal);
                   // cout<<"premise here"<<premise<<endl;
                    result =  backwardChaining(premise) & result;
                    //send this as query to backwardchaining and see if this again resolves to
                    //other premises..and repeat ,if not match with a liteal.
                }
            }




        }
    }else{
        //cout<<"conc= prem"<<endl;
        string second;
        bool lit = true;//true
        for(dependency_premise_it = dependency_premise.begin();dependency_premise_it !=  dependency_premise.end();dependency_premise_it++){

            premise = *dependency_premise_it;
           // cout<<"premiselit "<<premise<<endl;


            pos = premise.find("(");
            pemise_token = premise.substr(0,pos);
            innerpos = premise.find(",");
            literal_vect = predicate_literal_map[pemise_token];
            if(innerpos !=string::npos){
                pos = premise.find(",");
                innerpos = premise.find(")");
                second = premise.substr(pos+1,innerpos);
                if(!literal_vect.empty()){
                    // cout<<" till now.."<<endl;
                    for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                        premise =  *literal_vect_it;
                        pos = premise.find(",");
                        if(pos != string::npos){
                            innerpos = premise.find(")");
                            pemise_token = premise.substr(pos+1,innerpos);
                            if(pemise_token == second)
                                lit = lit & true;
                            else{
                                lit = lit & false;
                            }
                        }
                        else{
                            lit = lit & false;

                        }

                    }

                }
                else{
                    lit = false;
                }
                result = lit;
            }
            else{
                //only one to match...

                if(!literal_vect.empty()){

                    for(literal_vect_it = literal_vect.begin();literal_vect_it != literal_vect.end();literal_vect_it++){
                        premise =  *literal_vect_it;
                        pos = premise.find(",");
                        if(pos == string::npos){
                            lit =  lit & true;
                        }
                        else{
                            lit = lit & false;
                        }

                    }

                }
                else{
                    lit = false;
                }
                result = lit;

            }
        }
        //cout<<"res "<<result<<endl;
        // if premise literal is not found then
        if(!result){
            result = true;
            //call to revese backward chaining...
            //cout<<"yup "<<endl;
            for(dependency_premise_it = dependency_premise.begin();dependency_premise_it !=  dependency_premise.end();dependency_premise_it++){
                premise = *dependency_premise_it;

                result  = backwardChaining(premise) & result;
                //cout<<"yup premise "<<premise<<" "<<result<<endl;

            }


        }


    }
  //  cout<<"res finally "<<result<<endl;


    return result;
}
