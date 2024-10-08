/**
* @file         GUI.c
* @brief		GUI绘制相关源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/
 
#include "GUI.h"
#include "Lcd_Driver.h"
#include "Font.h"
#include "Picture.h"

/**
* @brief		画线函数，使用Bresenham 画线算法
* @param		x0 第一个点的横坐标
* @param		y0 第一个点的纵坐标
* @param		x1 第二个点的横坐标
* @param		y1 第二个点的纵坐标
* @return		无
*/
void Gui_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}



/**
* @brief		绘制GBK编码高16像素、宽8像素的英文、字符、数字；汉字的高、宽均为16像素
* @param		x 该字的左上角像素的横坐标
* @param		y 该字的左上角像素的纵坐标
* @param		fc 字的颜色
* @param		bc 字的背景颜色
* @param		s 要绘制的字符
* @return		无
*/
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, char *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;
	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}

/**
* @brief		绘制高16像素、宽8像素的数字
* @param		x 该数字的左上角像素的横坐标
* @param		y 该数字的左上角像素的纵坐标
* @param		fc 数字的颜色
* @param		bc 数字的背景颜色
* @param		num 要绘制的数字
* @return		无
*/
void Gui_DrawFont_Num16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
	unsigned char i,j;
	 for(i=0;i<16;i++)
		for(j=0;j<8;j++) 
			{
				if(asc16[(16+num)*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
				else 
				{
					if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
				}
			}
}



/**
* @brief		绘制长宽为32像素的数码管字体数字
* @param		x 该数字的左上角像素的横坐标
* @param		y 该数字的左上角像素的纵坐标
* @param		fc 数字的颜色
* @param		bc 数字的背景颜色
* @param		num 要绘制的数字
* @return		无
* @note			在电压电流表数字模式以及温湿度计功能中，显示数值用
*/
void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
	unsigned char i,j,k,c;
    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}


/**
* @brief		绘制16*16像素的图片
* @param		x 该图片左上角像素的横坐标
* @param		y 该图片左上角像素的纵坐标
* @param		p 图片名，在Picture.h中定义
* @return		无
* @note			用于绘制状态栏图标
*/
void showimage_16(const unsigned char *p, uint8_t x1,uint8_t y1) //显示16*16图片
{	
	uint16_t i;
	unsigned char picH,picL;
	Lcd_SetRegion(x1,y1,x1+13,y1+15);		//坐标设置
		for(i=0;i<16*16;i++)
	 {	
		picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		LCD_WriteData_16Bit(picH<<8|picL);  						
		}		
}



/**
* @brief		绘制系统LOGO
* @param		x LOGO的左上角像素的横坐标
* @param		y LOGO的左上角像素的纵坐标
* @return		无
* @note			LOGO分辨率为100*32
*/
void Show_Logo(uint8_t x1,uint8_t y1)
{
	const unsigned char *p = logo;
	uint16_t i;
	unsigned char picH,picL;
	Lcd_SetRegion(x1,y1,x1+97,y1+31);		//坐标设置
		for(i=0;i<100*32;i++)
	 {	
		picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		LCD_WriteData_16Bit(picH<<8|picL);  				
}
}

