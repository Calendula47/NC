//ui->textEditCode->append(str);这句话的textEditCode要你设置
//这个是绝对坐标cnc代码
void Widget::createCodeAbs(){
	int x = ui->lineEditX->text().toInt();//刀具原点x坐标
	int y = ui->lineEditY->text().toInt();//刀具原点y坐标
	int z = ui->lineEditZ->text().toInt();//刀具原点z坐标
	int s = ui->lineEditS->text().toInt();//转速
	int f = ui->movespeed->text().toInt();//进给速度
	float d = ui->lineEditD->text().toFloat();//刀具半径
	int choose;//判断曲线类型
	int way;//1为等间距2为等误差
	if (ui->choosetype->currentIndex() == 0)//判断曲线类型
	{//幂函数
		choose = 0;
	}
	else
	{//凸轮
		choose = 1;
	}
	if (ui->radioButton_equidistant->isChecked() == 1)
	{
	way = 1;
	}
	else
	{
	way = 2;
	}
	int rowNum = 0;//进行步数
	QString str = "%0001";//程序标题号
	ui->textEditCode->clear();
	ui->textEditCode->append(str);
	//开始cnc代码写出
	rowNum++;// 第一行CNC代码，设定程序参数
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G90 G92");
	str.append(" X");
	str.append(QString::number(x));
	str.append(" Y");
	str.append(QString::number(y));
	str.append(" Z");
	str.append(QString::number(z));
	str.append(" S");
	str.append(QString::number(s));
	str.append(" F");
	str.append(QString::number(f));
	if (ui->radioButton_MainS->isChecked())//这个可能也要你去设置一个正反转按钮
	{ //判断正反转
		str.append(" M03");
	}
	else if (ui->radioButton_MainN->isChecked())
	{
		str.append(" M04");
	}
	str.append(" M08");//冷却液
	ui->textEditCode->append(str);
	rowNum++;// CNC第二行代码
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G17 G00");//选定xy面
	if (ui->radioButton_KL->isChecked())//判断左右刀补
	{
		str.append(" G41");
	}
	else if (ui->radioButton_KR->isChecked())
	{
		str.append(" G42");
	}
	if (choose == 1)//判断曲线起点
	{ 
		if (way == 1)//判断凸轮的刀补方式
		{
		str.append(" X");
		str.append(QString::number(cam.NCX[0]));
		str.append(" Y");
		str.append(QString::number(cam.NCY[0]));
		}
		else{
		str.append(" X");
		str.append(QString::number(cam.NCX2[0]));
		str.append(" Y");
		str.append(QString::number(cam.NCY2[0]));
		}
	}
	else{
	str.append(" X");
	str.append(QString::number(MIHANSHUXIAOSHU().x_toolcompensate[0]));//已经改好了变量名
	str.append(" Y");
	str.append(QString::number(MIHANSHUXIAOSHU().y_toolcompensate[0]));
	}
	str.append(" D01");//刀号
	ui->textEditCode->append(str);
	rowNum++;// CNC第三行代码
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G01");
	str.append(" Z-");
	str.append(QString::number(d + 0.5));//厚度
	ui->textEditCode->append(str);
	//中间代码
	if (choose == 1)
	{
		if (way == 1)
		{	
		for (int i = 1; i<cam.NCX.length() - 0; i++)
		{
		rowNum++;//程序步数增加
		str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
		str.append(" X");
		str.append(QString::number(cam.NCX[i]));
		str.append(" Y");
		str.append(QString::number(cam.NCY[i]));
		ui->textEditCode->append(str);
		}
		}
		else{
		for (int i = 1; i<cam.NCX2.length() - 0; i++)
		{
		rowNum++;
		str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
		str.append(" X");
	str.append(QString::number(cam.NCX2[i]));
	str.append(" Y");
	str.append(QString::number(cam.NCY2[i]));
	ui->textEditCode->append(str);
		}
		}
	}	
	else
	{
	for (int i = 1; i<mishu.compensate_total; i++)
	{
	rowNum++;
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum% 1000) / 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum% 10) % 10);
	str.append(" X");
	str.append(QString::number(mishu.p_x_toolcompensate[i]));
	str.append(" Y");
	str.append(QString::number(mishu.p_y_toolcompensate[i]));
	ui->textEditCode->append(str);
	}
	}
	//CNc结束代码
	rowNum++;//
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G00");
	str.append(" Z");
	str.append(QString::number(z));
	ui->textEditCode->append(str);
	rowNum++;//步数+1
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G40");
	str.append(" X");
	str.append(QString::number(x));
	str.append(" Y");
	str.append(QString::number(y));
	str.append(" M05 M09");
	ui->textEditCode->append(str);
	rowNum++;//步数+1
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" M02");
	ui->textEditCode->append(str);
	}




void Widget::createCodes(){
	int x = ui->lineEditX->text().toInt();//刀具原点x坐标
	int y = ui->lineEditY->text().toInt();//刀具原点y坐标
	int z = ui->lineEditZ->text().toInt();//刀具原点z坐标
	int s = ui->lineEditS->text().toInt();//转速
	int f = ui->movespeed->text().toInt();//进给速度
	float d = ui->lineEditD->text().toFloat();//刀具半径
	int choose;//判断曲线类型
	int way;//1为等间距2为等误差
	float relx = 0;
	float rely = 0;
	if (ui->choosetype->currentIndex() == 0)//判断曲线类型
	{//幂函数
		choose = 0;
	}
	else
	{//凸轮
		choose = 1;
	}
	if (ui->radioButton_equidistant->isChecked() == 1)
	{
	way = 1;
	}
	else
	{
	way = 2;
	}
	int rowNum = 0;//进行步数
	QString str = "%0001";//程序标题号
	ui->textEditCode->clear();
	ui->textEditCode->append(str);
	//开始cnc代码写出
	rowNum++;// 第一行CNC代码，设定程序参数
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G91");
	str.append(" S");
	str.append(QString::number(s));
	str.append(" F");
	str.append(QString::number(f));
	if (ui->radioButton_MainS->isChecked())//这个可能也要你去设置一个正反转按钮
	{ //判断正反转
		str.append(" M03");
	}
	else if (ui->radioButton_MainN->isChecked())
	{
		str.append(" M04");
	}
	str.append(" M08");//冷却液
	ui->textEditCode->append(str);
	rowNum++;// CNC第二行代码
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G17 G00");//选定xy面
	if (ui->radioButton_KL->isChecked())//判断左右刀补
	{
		str.append(" G41");
	}
	else if (ui->radioButton_KR->isChecked())
	{
		str.append(" G42");
	}
	if (choose == 1)
	{ //
	if (way == 1)
	{
	relx = cam.NCX[0] - x;
	rely = cam.NCY[0] - y;
	str.append(" X");
	str.append(QString::number(relx));
	str.append(" Y");
	str.append(QString::number(rely));
	}
	else{
	relx = cam.NCX2[0] - x;
	rely = cam.NCY2[0] - y;
	str.append(" X");
	str.append(QString::number(relx));
	str.append(" Y");
	str.append(QString::number(rely));
	}
	}
	else{
	relx = mishu.p_x_toolcompensate[0] - x;
	rely = mishu.p_y_toolcompensate[0] - y；
	str.append(" X");
	str.append(QString::number(relx));
	str.append(" Y");
	str.append(QString::number(rely));
	}
	str.append(" D01");
	ui->textEditCode->append(str);
	rowNum++;// CNC
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G01");
	str.append(" Z-");
	str.append(QString::number(z + d + 0.5));
	ui->textEditCode->append(str);
	if (choose == 1)
	{//
	if (way == 1)
	{
	for (int i = 1; i<cam.NCX.length() - 0; i++){
	rowNum++;
	str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
	relx = fabs(cam.NCX[i] - cam.NCX[i - 1]);//相对坐标x
	rely = fabs(cam.NCY[i] - cam.NCY[i - 1]);//相对坐标y
	if (relx<0.000001)//四舍五入
	relx = 0.000001;
	else
	 relx = round(relx * pow(10, 6)) * pow(10, -6);
	if (rely<0.000001)
	rely = 0.000001;
	else
	 rely = round(rely * pow(10, 6)) * pow(10, -6);
	str.append(" X");
	if (cam.NCX[i]<cam.NCX[i - 1])//判断正负
	str.append("-");
	str.append(QString("%1").arg(relx, 0, 'q', 6));
	str.append(" Y");
	if (cam.NCY[i]<cam.NCY[i - 1])//判断正负
	str.append("-");
	str.append(QString("%1").arg(rely, 0, 'q', 6));
	ui->textEditCode->append(str);
	}
	}
	else{
	for (int i = 1; i<cam.NCX2.length() - 0; i++){
	rowNum++;
	str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
	relx = fabs(cam.NCX2[i] - cam.NCX2[i - 1]);//相对坐标x
	rely = fabs(cam.NCY2[i] - cam.NCY2[i - 1]);//相对坐标y
	if (relx<0.000001)//四舍五入
	relx = 0.000001;
	else 
	relx = round(relx * pow(10, 6)) * pow(10, -6);
	if (rely<0.000001)
	rely = 0.000001;
	else 
	rely = round(rely * pow(10, 6)) * pow(10, -6);
	str.append(" X");
	if (cam.NCX2[i]<cam.NCX2[i - 1])str.append("-");
	str.append(QString("%1").arg(relx, 0, 'q', 6));
	str.append(" Y");
	if (cam.NCY2[i]<cam.NCY2[i - 1])str.append("-");
	str.append(QString("%1").arg(rely, 0, 'q', 6));
	ui->textEditCode->append(str);
	}
	}
	}	
	//结束代码cnc
	rowNum++;
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" G00");
	str.append(" Z");
	str.append(QString::number(z + d + 0.5));
	ui->textEditCode->append(str);
	rowNum++;//
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	if (choose == 1)
	{
	if (way == 1)
	{
	relx = x - cam.NCX[0];
	rely = y - cam.NCY[0];
	}
	else
	{
	relx = x - cam.NCX2[0];
	rely = y - cam.NCY2[0];
	}
	}
	else{
	relx = x - MIHANSHUXIAOSHU().x_toolcompensate[MIHANSHUXIAOSHU().changdu_compensate];
	rely = y -MIHANSHUXIAOSHU().y_toolcompensate[MIHANSHUXIAOSHU().changdu_compensate];
	}
	str.append(" G40");
	str.append(" X");
	str.append(QString::number(relx));
	str.append(" Y");
	str.append(QString::number(rely));
	str.append(" M05 M09");
	ui->textEditCode->append(str);
	rowNum++;//
	str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
	str.append(" M02");
	ui->textEditCode->append(str);
	}
