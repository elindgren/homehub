package com.example.ericl.lighter;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class TouchBlackHoleView extends View {
    private boolean touch_disabled = false;

    public TouchBlackHoleView(Context context, AttributeSet attrs) {
        super(context, attrs);
        // this constructor used when creating view through XML
    }
    @Override
    public boolean onTouchEvent(MotionEvent e){
        return touch_disabled;
    }
    public void disable_touch(boolean b){
        touch_disabled=b;
    }
}
