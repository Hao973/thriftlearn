struct subjectscore
{
	1: required	string 	str_subject,       //科目
	2: required	i32    	i_score,		   //分数
}

struct student
{
	1: optional list<subjectscore> list_subscores,
	2: required i32 i_uid,                  //学号
	3: required string str_name,            //姓名
	4: optional string str_sex,             //性别
}

struct sturesult
{
	1: required i32 i_uid,                  //学号
	2: required string str_name,            //姓名
	3: optional string str_sex,             //性别
	4: required i32 i_subscore,			//总分
	5: required i32 i_avescore,			//平均分
	6: required string str_maxsub,			//最高分科目
	7: required string str_mixsub,			//最低分科目
}

service StudentServ
{
	sturesult work_score(1: student stu)
}