<h2> Infrence Engine using Backward Chaining</h2>
<p>
The engine accepts knowledge  in terms of FOL(First order Logic) sentences having up to two arguments.The KB(knowlede base) is all the sentences put together. Then you can ask a question to the engine, if the question is answerable given the knwoledge then engine will give  the ouptput as TRUE or FALSE, accordingly.
</p>


<h2> To Run the program use following I/O format</h2>

Input:
1st line consists of the question to be asked in terms of FOL. Followed by the no of senteneces (n) the KB needs to have.
Then n sentences in the form of FOL follow.
Output:
Consist of single line stating answer is True or False.

eg:
Input:
Diagnosis(John,Infected)
6
Works(x,Aidagency)&HasTraveled(x,Timbuktu)=>Diagnosis(x,Infected)
Diagnosis(x,Fever)=>HasSymptom(John,Fever)
HasSymptom(x,Fever)&HasTraveled(x,Timbuktu)=>Works(Alice,Aidagency)
Works(John,Aidagency)
Diagnosis(John,Fever)
HasTraveled(John,Timbuktu)

Output:
TRUE



